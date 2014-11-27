#include "bt_market_t.h"
#include "bt_task_t.h"
#include "bt_order_t.h"
#include "bt_orderlog_t.h"
#include "../strategy_base_t.h"
#include "bt_symbol_t.h"
#include "functions/functions.h"
#include "bt_deal_t.h"
#include "bt_robot_t.h"
#include "bt_orderbook_t.h"
#include "../trade_t.h"
#include "bt_deal_log_t.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <iostream>

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bt_market_t::bt_market_t(const string& filename) :
    m_robot{*this}
{
    m_market_events_file.open(filename);
    m_lg.add_attribute("time", m_time_attr);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_market_t::init_strategy(trading::strategy_base_t* strategy)
{
    m_orderlogs.emplace_back();
    m_deal_logs.emplace_back();
    m_orderbooks.emplace_back();
    m_orderbook_events.emplace_back(
        static_cast<const bt_symbol_t&>( strategy->symbol() ));
    m_tick_events.emplace_back(
        static_cast<const bt_symbol_t&>( strategy->symbol() ));
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

trading::robot_base_t& bt_market_t::robot()
{
    return m_robot;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_market_t::run()
{
    while (true)
    {
        market_event_t& market_event = get_market_event();

        if (market_event.datetime == ptime{max_date_time})
            break;

        while(!m_tasks.empty())
        {
            if (m_tasks.front()[0].datetime <= market_event.datetime)
            {
                auto tasks = m_tasks.front();
                m_tasks.pop();
                m_datetime = tasks[0].datetime;
                for (auto& task : tasks)
                {
                    task.execute();
                }

                //for (auto& strategy : m_strategies)
                //    strategy->process();
                process_strategies();
            }
            else
                break;
        }

        m_datetime = market_event.datetime;

        switch ( market_event.type )
        {
        case event_type_t::orderbook:
            handle_orderbook_event( static_cast<orderbook_event_t&>(market_event) );
            break;

        case event_type_t::tick:
            handle_tick_event( static_cast<tick_event_t&>(market_event) );
            break;

        default:
            break;
        }

        //for (auto& strategy : m_strategies)
        //    strategy->process();
        process_strategies();
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_market_t::add_task(const ptime& datetime, const function<void()>& task)
{
    if (m_tasks.empty())
    {
        m_tasks.push( vector<bt_task_t>({ bt_task_t{datetime, task} }) );
        return;
    }

    if (datetime == m_tasks.front()[0].datetime)
        m_tasks.front().emplace_back(datetime, task);
    else
        m_tasks.push( vector<bt_task_t>({ bt_task_t{datetime, task} }) );
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_market_t::add_order(const bt_order_t& _order, bool call_callback)
{
    bt_order_t order{_order};
    order.datetime(m_datetime);
    const bt_symbol_t& symbol = static_cast<const bt_symbol_t&>(order.symbol());
    int pos = get_position(symbol);

    trading::strategy_base_t& strategy = *m_strategies[pos];
    bt_orderlog_t& orderlog = m_orderlogs[pos];
    bt_orderbook_t& orderbook = m_orderbooks[pos];

    vector<bt_deal_t> deals;
    vector<bt_order_t> orders;
    tie(orders, orderbook, deals) = functions::add_order(order, orderbook);

    add_task(
        task_time(),
        [&strategy, orderbook]() {
            bt_orderbook_t& bt_orderbook =
                    static_cast<bt_orderbook_t&>(strategy.orderbook());
            bt_orderbook = orderbook;
    });

    bt_order_t added_order{order};
    added_order.volume(0);
    added_order.orders_before(0);

    for (const auto& order : orders)
    {
        orderlog.add(order);
        if (added_order.id() == order.id())
        {
            added_order.volume( order.volume() );
            added_order.orders_before( order.orders_before() );
        }
        else
        {
            BT_LOG() << "BACKTESTER find added_order error";
        }
    }

    if (!orders.empty())
    {
        add_task(
            task_time(),
            [&strategy, orderlog]() {
                bt_orderlog_t& bt_orderlog =
                        static_cast<bt_orderlog_t&>(strategy.orderlog());
                bt_orderlog = orderlog;
        });
    }

    if (call_callback)
    {
        BT_LOG() << "BACKTESTER [add_order]: "
                 << "id=" << order.id() << " "
                 << dir_str[static_cast<int>(order.dir())]
                 << " price=" << order.price()
                 << " volume=" << order.volume();

        add_task(
            task_time(),
            [&strategy, added_order]() {
                strategy.order_succeeded(added_order);
        });
    }

    for (const auto& deal : deals )
    {
        add_deal(deal);
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_market_t::del_order(const bt_order_t& order, bool call_callback)
{
    const bt_symbol_t& symbol = static_cast<const bt_symbol_t&>(order.symbol());
    int pos = get_position(symbol);
    if (pos == -1)
        return;

    trading::strategy_base_t& strategy = *m_strategies[pos];

    bt_orderlog_t& orderlog = m_orderlogs[pos];
    auto& bids = orderlog.m_bids;
    auto& asks = orderlog.m_asks;

    auto cond = [&order] (const bt_order_t& _order_) -> bool
    {
        return _order_.id() == order.id();
    };

    bool succeeded = false;
    bt_order_t del_order{order};

    switch ( order.dir() )
    {
    case order_dir_t::bid:
    {
        auto it = find_if(bids.begin(), bids.end(), cond);
        if (it != bids.end())
        {
            succeeded = true;
            del_order = *it;
            bids.erase(it);
        }

    } break;

    case order_dir_t::ask:
    {
        auto it = find_if(asks.begin(), asks.end(), cond);
        if (it != asks.end())
        {
            succeeded = true;
            del_order = *it;
            asks.erase(it);
        }

    } break;

    default:
        throw -1;
    }

    if (succeeded)
    {
        add_task(
            task_time(),
            [&strategy, orderlog]() {
                bt_orderlog_t& bt_orderlog =
                        static_cast<bt_orderlog_t&>(strategy.orderlog());
                bt_orderlog = orderlog;
        });

        if (call_callback)
        {
            BT_LOG() << "BACKTESTER [del_order] succeeded: id=" << order.id();

            add_task(
                task_time(),
                [&strategy, del_order]() {
                    strategy.order_del_succeeded(del_order);
            });
        }
    }
    else
    {
        if (call_callback)
        {
            BT_LOG() << "BACKTESTER [del_order] failed: id=" << order.id();

            add_task(
                task_time(),
                [&strategy, del_order]() {
                    strategy.order_del_failed(del_order);
            });
        }
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_market_t::move_order(const bt_order_t& order, double price)
{
    const bt_symbol_t& symbol = static_cast<const bt_symbol_t&>(order.symbol());
    int pos = get_position(symbol);

    trading::strategy_base_t& strategy = *m_strategies[pos];

    bt_orderbook_t& orderbook = m_orderbooks[pos];
    bt_orderlog_t& orderlog = m_orderlogs[pos];
    auto& bids = orderlog.m_bids;
    auto& asks = orderlog.m_asks;

    auto cond = [&order] (const bt_order_t& _order_) -> bool
    {
        return _order_.id() == order.id();
    };

    bool succeeded = false;

    vector<bt_order_t>::iterator it;
    if (order.dir() == order_dir_t::bid)
    {
        it = find_if(bids.begin(), bids.end(), cond);
        if (it != bids.end())
            succeeded = true;
    }
    else
    {
        it = find_if(asks.begin(), asks.end(), cond);
        if (it != asks.end())
            succeeded = true;
    }

    if (succeeded)
    {
        BT_LOG() << "BACKTESTER [move_order] succeeded: id=" << order.id();

        bt_order_t _order = *it;
        _order.price(price);
        _order.orders_before( orderbook.orders_before(_order) );

        add_task(
            task_time(),
            [&strategy, _order]() {
                strategy.order_move_succeeded(_order);
        });

        bt_order_t move_order{*it};
        del_order(move_order, false);
        move_order.price(price);
        add_order(move_order, false);
    }
    else
    {
        BT_LOG() << "BACKTESTER [move_order] failed: id=" << order.id();

        add_task(
            task_time(),
            [&strategy, order]() {
                strategy.order_move_failed(order);
        });
    }


//    if (succeeded)
//    {
//        move_order = *it;
//        del_order(move_order, false);
//        move_order.price(price);
//        add_order(move_order, false);

//        bool released = true;
//        if (order.dir() == order_dir_t::bid)
//        {
//            it = find_if(bids.begin(), bids.end(), cond);
//            if (it != bids.end())
//                released  = false;
//        }
//        else
//        {
//            it = find_if(asks.begin(), asks.end(), cond);
//            if (it != asks.end())
//                released  = false;
//        }

//        if (released)
//        {
//            move_order.orders_before( 0 );
//            move_order.volume( 0 );
//        }
//        else
//        {
//            move_order.orders_before( it->orders_before() );
//            move_order.volume( it->volume() );
//        }
//    }

//    switch ( order.dir() )
//    {
//    case order_dir_t::bid:
//    {
//        auto it = find_if(bids.begin(), bids.end(), cond);
//        if (it != bids.end())
//        {
//            succeeded = true;
//            move_order = *it;
//            del_order(move_order, false);
//            move_order.price(price);
//            add_order(move_order, false);
//        }

//    } break;

//    case order_dir_t::ask:
//    {
//        auto it = find_if(asks.begin(), asks.end(), cond);
//        if (it != asks.end())
//        {
//            succeeded = true;
//            move_order = *it;
//            del_order(move_order, false);
//            move_order.price(price);
//            add_order(move_order, false);
//        }

//    } break;

//    default:
//        return;
//    }

//    if (succeeded)
//    {
//        BT_LOG() << "BACKTESTER [move_order] succeeded: id=" << order.id();

//        add_task(
//            task_time(),
//            [&strategy, move_order]() {
//                strategy.order_move_succeeded(move_order);
//        });
//    }
//    else
//    {
//        BT_LOG() << "BACKTESTER [move_order] failed: id=" << order.id();

//        add_task(
//            task_time(),
//            [&strategy, move_order]() {
//                strategy.order_move_failed(move_order);
//        });
//    }

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_market_t::add_deal(const bt_deal_t& deal)
{
    const bt_symbol_t& symbol = static_cast<const bt_symbol_t&>(deal.symbol());
    int pos = get_position(symbol);

    trading::strategy_base_t& strategy = *m_strategies[pos];
    bt_deal_log_t& deal_log = m_deal_logs[pos];

    vector<double> profits;
    tie(deal_log, profits) = functions::add_deal(deal, deal_log);

    add_task(
        task_time(),
        [&strategy, deal_log]() {
            bt_deal_log_t& bt_deal_log =
                    static_cast<bt_deal_log_t&>(strategy.deal_log());
            bt_deal_log = deal_log;
    });

    BT_LOG() << "BACKTESTER [add_deal]:"
             << " order id=" << deal.order_id()
             << " price=" << deal.price()
             << " volume=" << deal.volume();

    ptime _datetime{m_datetime};
    add_task(
        task_time(),
        [&strategy, deal, profits, _datetime]() {
            for (const auto profit : profits)
                strategy.add_trade( trade_t{profit, _datetime} );
            strategy.add_deal(deal);
    });
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

market_event_t &bt_market_t::get_market_event()
{
    using boost::algorithm::trim;
    using boost::posix_time::time_from_string;

    string str_event;
    do
    {
        std::getline(m_market_events_file, str_event);
        if (m_market_events_file.eof())
        {
            m_tick_events[0].datetime = ptime{max_date_time};
            return m_tick_events[0];
        }
        trim(str_event);
    } while( str_event.empty() );

    stringstream ss{str_event};
    string item;

    //--------------------------------------------

    orderbook_event_t& orderbook_event =
            m_orderbook_events[ get_position("") ];
    tick_event_t& tick_event =
            m_tick_events[ get_position("") ];

    //--------------------------------------------

    getline(ss, item, ';');
    if (item.size() >= 4)
    {
        stringstream ss;
        ss << item[3];
        item = ss.str();
    }
    trim(item);
    //std::cout << "item=\"" << item.size() << "\"\n";

    if (item[0] == 't')
    {
        getline(ss, item, ';');
        tick_event.datetime = time_from_string(item);
//        std::cout << item << "\t"
//                  << boost::posix_time::to_simple_string(tick_event.datetime) << "\n";

        getline(ss, item, ';');
        tick_event.price = std::stod(item);

        getline(ss, item, ';');
        int volume = std::stoi(item);
        if (volume > 0) // SELL
        {
            tick_event.volume = volume;
            tick_event.tick_type = deal_type_t::sell;
        }
        else // BUY
        {
            tick_event.volume = -volume;
            tick_event.tick_type = deal_type_t::buy;
        }
        return tick_event;
    }
    else if (item[0] == 's')
    {
        getline(ss, item, ';');
        orderbook_event.datetime = time_from_string(item);
//        std::cout << item << "\t"
//                  << boost::posix_time::to_simple_string(orderbook_event.datetime) << "\n";

        auto& bids = orderbook_event.bids;
        bids.clear();
        auto& asks = orderbook_event.asks;
        asks.clear();

        double price = 0;
        int volume = 0;
        for (int i = 0; getline(ss, item, ','); ++i)
        {
            if (i%2 == 0)
                price = std::stod(item);
            else
            {
                volume = std::stoi(item);
                if (volume > 0)
                    bids[price] = volume;
                else
                    asks[price] = -volume;
            }
        }
        return orderbook_event;
    }
    else
    {
        std::cout << "parse event exception" << "\n";
        std::cout << "event=\"" << item << "\"\n";
        throw -1;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_market_t::handle_orderbook_event(const orderbook_event_t& orderbook_event)
{
    int pos = get_position( static_cast<const bt_symbol_t&>(orderbook_event.symbol) );
    if (pos == -1)
        return;

    trading::strategy_base_t& strategy = *m_strategies[pos];
    bt_orderbook_t& orderbook = m_orderbooks[pos];
    bt_orderlog_t& orderlog = m_orderlogs[pos];

    orderbook.set_bids( orderbook_event.bids );
    orderbook.set_asks( orderbook_event.asks );

    auto bids = orderlog.bids();
    for (size_t i = 0; i < bids.size(); i++)
    {
        bt_order_t& order = static_cast<bt_order_t&>(bids[i]);
        if (orderbook[order.price()] < order.orders_before())
            order.orders_before( orderbook[order.price()] );
    }

    auto asks = orderlog.asks();
    for (size_t i = 0; i < asks.size(); i++)
    {
        bt_order_t& order = static_cast<bt_order_t&>(asks[i]);
        if (orderbook[order.price()] < order.orders_before())
            order.orders_before( orderbook[order.price()] );
    }

    add_task(
        task_time(),
        [&strategy, orderbook, orderlog]() {
            bt_orderbook_t& bt_orderbook =
                    static_cast<bt_orderbook_t&>(strategy.orderbook());
            bt_orderbook = orderbook;

            strategy.update_orderbook();

            bt_orderlog_t& bt_orderlog =
                    static_cast<bt_orderlog_t&>(strategy.orderlog());
            bt_orderlog = orderlog;

            strategy.update_orders_before();
    });
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_market_t::handle_tick_event(const tick_event_t& tick_event)
{
    int pos = get_position( static_cast<const bt_symbol_t&>(tick_event.symbol) );
    if (pos == -1)
        return;

    trading::strategy_base_t& strategy = *m_strategies[pos];
    bt_orderlog_t& orderlog = m_orderlogs[pos];
    bt_orderbook_t& orderbook = m_orderbooks[pos];

    bt_deal_t tick{static_cast<const bt_symbol_t&>(tick_event.symbol),
                   tick_event.tick_type,
                   tick_event.price,
                   tick_event.volume,
                   tick_event.datetime};

    vector<bt_deal_t> deals;
    tie(orderlog, deals) = functions::handle_tick_event(tick, orderlog);

    if (tick.type() == deal_type_t::buy)
    {
        orderbook.set(order_dir_t::ask,
                      tick.price(),
                      orderbook.asks[tick.price()] - tick.volume());

//        for (double price = tick.price() - tick.symbol().step();
//             price >= orderbook.best_ask();
//             price -= tick.symbol().step())
//            orderbook.asks.set(price, 0);
    }
    else
    {
        orderbook.set(order_dir_t::bid,
                      tick.price(),
                      orderbook.bids[tick.price()] - tick.volume());

//        for (double price = tick.price() + tick.symbol().step();
//             price <= orderbook.best_bid();
//             price += tick.symbol().step())
//            orderbook.bids.set(price, 0);
    }

    add_task(
        task_time(),
        [&strategy, orderlog, tick, orderbook]() {
            strategy.add_tick(tick);

            bt_orderbook_t& bt_orderbook =
                    static_cast<bt_orderbook_t&>(strategy.orderbook());
            bt_orderbook = orderbook;

            strategy.update_orderbook();

            bt_orderlog_t& bt_orderlog =
                    static_cast<bt_orderlog_t&>(strategy.orderlog());
            bt_orderlog = orderlog;

            strategy.update_orders_before();
    });

    for (const auto& deal : deals )
    {
        add_deal(deal);
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int bt_market_t::get_position(const bt_symbol_t& symbol)
{
    return 0;
//    for (int i = 0; i < m_strategies.size(); i++)
//        if (symbol.equal( m_strategies[i].symbol() ))
//            return i;
    //    return -1;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int bt_market_t::get_position(const string& symbol_name)
{
    return 0;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
