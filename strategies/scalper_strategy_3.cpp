#include "scalper_strategy_3.h"
#include "scalper_order_t.h"
#include "../market_base_t.h"
#include "../deal_base_t.h"
#include "../utils.h"
#include "../orderbook_base_t.h"
#include "../symbol_base_t.h"
#include "../orderlog_base_t.h"
#include "../order_t.h"
#include "../types.h"
#include "../utils.h"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/utility/empty_deleter.hpp>

#include <iostream>

namespace strategies
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

scalper_strategy_3_t::scalper_strategy_3_t(
        market_base_t& market,
        symbol_base_t& symbol,
        marketdata_base_t& marketdata) :

    strategy_base_t{market, symbol, marketdata}

{
    m_lg.add_attribute("pid", attr::constant<scalper_strategy_3_t*>(this));
    m_lg.add_attribute("time", m_time_attr);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

scalper_strategy_3_t::~scalper_strategy_3_t()
{
    m_lg.remove_all_attributes();
    boost::shared_ptr< logging::core > core = logging::core::get();
    core->remove_sink(m_sink);
    write_stat();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::order_succeeded(const order_base_t& order)
{
    STRATEGY_LOG(m_lg, debug) << "CALLBACK [order_succeeded] id=" << order.id() << " "
                              << dir_str[static_cast<int>(order.dir())]
                              << " price=" << order.price()
                              << " volume=" << order.volume()
                              << " orders_before=" << order.orders_before();

    int trade_index;
    int order_index;
    try {
        tie(trade_index, order_index) = find_order( order.id() );
    }
    catch (...) {
        STRATEGY_LOG(m_lg, debug) << "error of [order_succeeded]: "
                                  << "there isn't order with id=" << order.id();
        return;
    }

    auto& trade = m_trades[trade_index];
    auto& orders = trade.orders;
    auto& added_order = orders[order_index];

    added_order.orders_before( order.orders_before() );
    added_order.datetime( order.datetime() );
    added_order.stop_pending();

    scalper_order_t event{added_order};
    event.datetime(m_market.datetime());
    trade.event_writer().add_order_event(event);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::order_failed(const order_base_t& order)
{
    STRATEGY_LOG(m_lg, debug) << "CALLBACK [order_failed] id=" << order.id();

    int trade_index;
    int order_index;
    try {
        tie(trade_index, order_index) = find_order( order.id() );
    }
    catch (...) {
        STRATEGY_LOG(m_lg, debug) << "error of [order_failed]: "
                                  << "there isn't order with id=" << order.id();
        return;
    }

    auto& trade = m_trades[trade_index];
    auto& orders = trade.orders;

    orders.erase( orders.begin()+order_index );
    trade.cancel();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::order_del_succeeded(const order_base_t& order)
{
    STRATEGY_LOG(m_lg, debug) << "CALLBACK [order_del_succeeded] id=" << order.id();

    int trade_index;
    int order_index;
    try {
        tie(trade_index, order_index) = find_order( order.id() );
    }
    catch (...) {
        STRATEGY_LOG(m_lg, debug) << "error of [order_del_succeeded]: "
                                  << "there isn't order with id=" << order.id();
        return;
    }

    auto& trade = m_trades[trade_index];
    auto& orders = trade.orders;
    auto& del_order = orders[order_index];

    // событыие о том, что заявка удалена
    scalper_order_t event{del_order};
    event.datetime(m_market.datetime());
    event.volume(0);
    trade.event_writer().add_order_event(event);

    // удаление заявки
    orders.erase( orders.begin()+order_index );
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::order_del_failed(const order_base_t& order)
{
    STRATEGY_LOG(m_lg, debug) << "CALLBACK [order_del_failed] id=" << order.id();

    int trade_index;
    int order_index;
    try {
        tie(trade_index, order_index) = find_order( order.id() );
    }
    catch (...) {
        STRATEGY_LOG(m_lg, debug) << "error of [order_del_failed]: "
                                  << "there isn't order with id=" << order.id();
        return;
    }

    auto& trade = m_trades[trade_index];
    auto& orders = trade.orders;
    auto& del_order = orders[order_index];

    del_order.stop_pending();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::order_move_succeeded(const order_base_t& order)
{
    STRATEGY_LOG(m_lg, debug) << "CALLBACK [order_move_succeeded] id=" << order.id();

    int trade_index;
    int order_index;
    try {
        tie(trade_index, order_index) = find_order( order.id() );
    }
    catch (...) {
        STRATEGY_LOG(m_lg, debug) << "error of [order_move_succeeded]: "
                                  << "there isn't order with id=" << order.id();
        return;
    }

    auto& trade = m_trades[trade_index];
    auto& orders = trade.orders;
    auto& moved_order = orders[order_index];

    // событие о перемещении заявки
    scalper_order_t event{moved_order};
    event.datetime(m_market.datetime());
    event.volume(0);
    trade.event_writer().add_order_event(event);

    // новая цена заявки и orders_before
    moved_order.stop_pending();
    moved_order.price( order.price() );
    moved_order.orders_before( order.orders_before() );

    // событие о том, что заявка перемещена
    event.price(moved_order.price());
    event.volume(moved_order.volume());
    event.orders_before(moved_order.orders_before());
    trade.event_writer().add_order_event(event);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::order_move_failed(const order_base_t& order)
{
    STRATEGY_LOG(m_lg, debug) << "CALLBACK [order_move_failed] id=" << order.id();

    int trade_index;
    int order_index;
    try {
        tie(trade_index, order_index) = find_order( order.id() );
    }
    catch (...) {
        STRATEGY_LOG(m_lg, debug) << "error of [order_move_failed]: "
                                  << "there isn't order with id=" << order.id();
        return;
    }

    auto& trade = m_trades[trade_index];
    auto& orders = trade.orders;
    auto& moved_order = orders[order_index];

    moved_order.stop_pending();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::add_deal(const deal_base_t& deal)
{
    auto const order_id = deal.order_id();

    int trade_index;
    int order_index;
    try {
        tie(trade_index, order_index) = find_order( order_id );
    }
    catch (...) {
        STRATEGY_LOG(m_lg, debug) << "error of [add_deal]: "
                                  << "there isn't order with id=" << order_id;
        return;
    }

    auto& trade = m_trades[trade_index];
    auto& orders = trade.orders;
    auto& order = orders[order_index];

    STRATEGY_LOG(m_lg, debug) << "CALLBACK [add_deal]: "
                              << "order id=" << order_id << " "
                              << dir_str[static_cast<int>(order.dir())]
                              << " price=" << deal.price()
                              << " volume=" << deal.volume();

    deal_base_t event{deal};
    event.datetime(m_market.datetime());
    trade.event_writer().add_deal_event(event);

    trade.add_deal(deal);

    if ( !trade.is_started() )
    {
        if ( order.dir() == order_dir_t::bid )
            trade.ask_price( trade.bid_price() + m_symbol.step() );
        else
            trade.bid_price( trade.ask_price() - m_symbol.step() );

        trade.start(m_market.datetime());

        STRATEGY_LOG(m_lg, debug) << "should start trade (after deal): "
                                  << "bid=" << trade.bid_price() << " "
                                  << "ask=" << trade.ask_price();

        //std::cout << "id=" << trade.id() << std::endl;

        trade.set_first_order(order.dir());

        trade.set_sell_volume_5s();
        trade.set_buy_volume_5s();
        trade.set_sell_ticks_5s();
        trade.set_buy_ticks_5s();

        trade.set_bid_volume_5s();
        trade.set_bid_volume_100ms();
        trade.set_ask_volume_5s();
        trade.set_ask_volume_100ms();
    }

    if ((trade.buy_volume() == trade.sell_volume()) && (trade.orders.size() == 0))
    {
        STRATEGY_LOG(m_lg, info) << "trade complete " << m_tradelog.total();
        trade.complete();
    }

    order.volume( order.volume() - deal.volume() );
    if ( order.volume() <= 0 )
        orders.erase( orders.begin()+order_index );
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::add_tick(const deal_base_t& tick)
{
    while (!m_tick_events.empty())
    {
        const auto& first = m_tick_events.front();
        if (m_market.datetime() - first.datetime > seconds(5))
            m_tick_events.pop_front();
        else
            break;
    }

    tick_event_t event{m_symbol};
    event.datetime = m_market.datetime();
    event.price = tick.price();
    event.volume = tick.volume();
    event.tick_type = tick.type();

    for (scalper_trade_t& trade : m_trades)
    {
        if (!trade.event_writer().is_tick_init())
            trade.event_writer().init_tick(m_tick_events);

        trade.event_writer().add_tick_event(event);
    }

    m_tick_events.push_back(event);
    m_all_tick_events.push_back(event);

    if (tick.type() == deal_type_t::sell)
        m_last_sell_tick = tick.price();
    else
        m_last_buy_tick = tick.price();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::process()
{
    const strategy_state_t cur_state = get_state();

    if (m_prev_state != cur_state)
    {
        switch ( cur_state )
        {
        case strategy_state_t::work:
            STRATEGY_LOG(m_lg, info) << "START TRADING";
            break;

        case strategy_state_t::prepare_to_stop:
            STRATEGY_LOG(m_lg, info) << "PREPARE TO STOP TRADING";
            break;

        case strategy_state_t::stop:
            STRATEGY_LOG(m_lg, info) << "STOP TRADING";
            break;
        }
    }

    switch ( cur_state )
    {
    case strategy_state_t::work:
        work();
        break;

    case strategy_state_t::prepare_to_stop:
        prepare_to_stop();
        break;

    case strategy_state_t::stop:
        break;

    default:
        throw -1;
    }

    m_prev_state = cur_state;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------


void scalper_strategy_3_t::update_orders_before()
{
    map<int, int> ob;

    auto bids = m_orderlog.bids();
    for (size_t i = 0; i < bids.size(); i++)
        ob[ bids[i].id() ] = bids[i].orders_before();

    auto asks = m_orderlog.asks();
    for (size_t i = 0; i < asks.size(); i++)
        ob[ asks[i].id() ] = asks[i].orders_before();

    for (auto& trade : m_trades)
    {
        for (auto& order : trade.orders)
        {
            try
            {
                if ( order.orders_before() > ob.at(order.id()) )
                {
                    order.orders_before( ob.at(order.id()) );
                    scalper_order_t event{order};
                    event.datetime(m_market.datetime());
                    trade.event_writer().add_order_event(event);
                }
            }
            catch (...)
            {
                STRATEGY_LOG(m_lg, info) << "THROW [update_orders_before]";
            }
        }
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::update_orderbook()
{
    while (!m_orderbook_events.empty())
    {
        const auto& first = m_orderbook_events.front();
        if (m_market.datetime() - first.datetime > seconds(5))
            m_orderbook_events.pop_front();
        else
            break;
    }

    orderbook_event_t event{m_symbol};
    event.datetime = m_market.datetime();
    event.bids = m_orderbook.get_bids();
    event.asks = m_orderbook.get_asks();

    for (scalper_trade_t& trade : m_trades)
    {
        if (!trade.event_writer().is_stocks_init())
            trade.event_writer().init_stocks(m_orderbook_events);

        trade.event_writer().add_stocks_event(event);
    }

    m_orderbook_events.push_back(event);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::init(scalper_trade_t& trade)
{
    STRATEGY_LOG(m_lg, info) << "CMD: trade init";
    trade.create_time(m_market.datetime());

    int quotes_amount = 12;
    vector<quote_t> quotes;
    for (int i = 0; i <= quotes_amount; i++)
    {
        double price = m_orderbook.best_bid() - i * m_symbol.step();
        quotes.push_back( quote_t{price, m_orderbook[price]} );
    }
    std::stable_sort(quotes.begin(), quotes.end(),
              [](const quote_t& quote1, const quote_t& quote2)
              {
                  return quote1.volume > quote2.volume;
              });
    double price = quotes[0].price + m_symbol.step();
    price = m_last_sell_tick - trade.distance()*m_symbol.step();
    trade.bid_price(price);
    add_order(trade, scalper_order_t{m_symbol, order_dir_t::bid, price, 1});

    quotes.clear();
    for (int i = 0; i <= quotes_amount; i++)
    {
        double price = m_orderbook.best_ask() + i * m_symbol.step();
        quotes.push_back( quote_t{price, m_orderbook[price]} );
    }
    std::stable_sort(quotes.begin(), quotes.end(),
              [](const quote_t& quote1, const quote_t& quote2)
              {
                  return quote1.volume > quote2.volume;
              });
    price = quotes[0].price - m_symbol.step();
    price = m_last_buy_tick + trade.distance()*m_symbol.step();
    trade.ask_price(price);
    add_order(trade, scalper_order_t{m_symbol, order_dir_t::ask, price, 1});
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bool scalper_strategy_3_t::try_to_start(scalper_trade_t& trade)
{
//    if (m_market.datetime() - trade.create_time() >= seconds(20))
//    {
//        trade.bid_price( trade.bid_price()+m_symbol.step() );
//        trade.ask_price( trade.ask_price()-m_symbol.step() );
//        trade.create_time( m_market.datetime() );
//        return false;
//    }

    for (scalper_order_t& order : trade.orders)
    {
        bool should_start = false;

        if (order.dir() == order_dir_t::bid)
        {
            if (m_last_sell_tick - order.price() <= 10)
            {
                if (0.5*orderbook_volume(order_dir_t::ask) > orderbook_volume(order_dir_t::bid))
                {
                    if (0.5*tick_volume(deal_type_t::sell, seconds(20)) > tick_volume(deal_type_t::buy, seconds(20)))
                    {
                        if (!(orders_before(order,1)+orders_after(order,1) > 3*attack_orders(order, 5)))
                            should_start = true;
                    }
                }
            }
        }
        else
        {
            if (order.price() - m_last_buy_tick <= 10)
            {
                if (orderbook_volume(order_dir_t::ask) < 0.5*orderbook_volume(order_dir_t::bid))
                {
                    if (tick_volume(deal_type_t::sell, seconds(20)) < 0.5*tick_volume(deal_type_t::buy, seconds(20)))
                    {
                        if (!(orders_before(order,1)+orders_after(order,1) > 3*attack_orders(order, 5)))
                            should_start = true;
                    }
                }
            }
        }



        if (order.dir() == order_dir_t::bid)
        {
            if (m_last_sell_tick - order.price() <= 10)
            {
                if (0.25*tick_volume(deal_type_t::sell, seconds(10)) > tick_volume(deal_type_t::buy, seconds(10)))
                {
                    should_start = true;
                }
            }
        }
        else
        {
            if (order.price() - m_last_buy_tick <= 10)
            {
                if (tick_volume(deal_type_t::sell, seconds(10)) < 0.25*tick_volume(deal_type_t::buy, seconds(10)))
                {
                    should_start = true;
                }
            }
        }




//        if (order.dir() == order_dir_t::bid)
//        {
//            if (m_last_sell_tick - order.price() <= 10)
//            {
//                if (0.8*orderbook_volume(order_dir_t::ask) > orderbook_volume(order_dir_t::bid))
//                {
//                    if (0.1*tick_volume(deal_type_t::sell, seconds(10)) > tick_volume(deal_type_t::buy, seconds(10)))
//                    {
//                        if (!(orders_before(order,1)+orders_after(order,1) > 3*attack_orders(order, 5)))
//                            should_start = true;
//                    }
//                }
//            }
//        }
//        else
//        {
//            if (order.price() - m_last_buy_tick <= 10)
//            {
//                if (orderbook_volume(order_dir_t::ask) < 0.8*orderbook_volume(order_dir_t::bid))
//                {
//                    if (tick_volume(deal_type_t::sell, seconds(10)) < 0.1*tick_volume(deal_type_t::buy, seconds(10)))
//                    {
//                        if (!(orders_before(order,1)+orders_after(order,1) > 3*attack_orders(order, 5)))
//                            should_start = true;
//                    }
//                }
//            }
//        }

//        if (order.dir() == order_dir_t::bid)
//        {
//            if (m_last_sell_tick - order.price() <= 10)
//            {
//                if (0.2*orderbook_volume(order_dir_t::ask) > orderbook_volume(order_dir_t::bid))
//                {
//                    if (0.8*tick_volume(deal_type_t::sell, seconds(10)) > tick_volume(deal_type_t::buy, seconds(10)))
//                    {
//                        if (!(orders_before(order,1)+orders_after(order,1) > 3*attack_orders(order, 5)))
//                            should_start = true;
//                    }
//                }
//            }
//        }
//        else
//        {
//            if (order.price() - m_last_buy_tick <= 10)
//            {
//                if (orderbook_volume(order_dir_t::ask) < 0.2*orderbook_volume(order_dir_t::bid))
//                {
//                    if (tick_volume(deal_type_t::sell, seconds(10)) < 0.8*tick_volume(deal_type_t::buy, seconds(10)))
//                    {
//                        if (!(orders_before(order,1)+orders_after(order,1) > 3*attack_orders(order, 5)))
//                            should_start = true;
//                    }
//                }
//            }
//        }

//        if (order.dir() == order_dir_t::bid)
//        {
//            if (m_last_sell_tick - order.price() <= 20)
//            {
//                if (0.3*orderbook_volume(order_dir_t::ask) > orderbook_volume(order_dir_t::bid))
//                {
//                    if (0.3*tick_volume(deal_type_t::sell, seconds(10)) > tick_volume(deal_type_t::buy, seconds(10)))
//                    {
//                        if (!(orders_before(order,1)+orders_after(order,1) > 3*attack_orders(order, 5)))
//                            should_start = true;
//                    }
//                }
//            }
//        }
//        else
//        {
//            if (order.price() - m_last_buy_tick <= 20)
//            {
//                if (orderbook_volume(order_dir_t::ask) < 0.3*orderbook_volume(order_dir_t::bid))
//                {
//                    if (tick_volume(deal_type_t::sell, seconds(10)) < 0.3*tick_volume(deal_type_t::buy, seconds(10)))
//                    {
//                        if (!(orders_before(order,1)+orders_after(order,1) > 3*attack_orders(order, 5)))
//                            should_start = true;
//                    }
//                }
//            }
//        }




//        if ( (levels_before(order) <= 3) &&
//             (orders_after(order, 1) <= 800) )
//        {
//            del_order(order);
//            trade.cancel();
//            STRATEGY_LOG(m_lg, debug) << "trade cancel #1";
//            return false;
//        }

//        if ( (attack_orders(order, 2) >= 20) &&
//             (orders_after(order, 1) <= 200) )
//        {
//            del_order(order);
//            trade.cancel();
//            STRATEGY_LOG(m_lg, debug) << "trade cancel #2";
//            return false;
//        }

//        if ( (levels_before(order) <= 2) &&
//             (attack_orders(order, 1) <= 0) &&
//             (orders_after(order, 1) <= 10) )
//        {
//            should_start = true;
//        }

//        should_start = true;

        if (should_start)
        {
            if (order.dir() == order_dir_t::bid)
                trade.ask_price( trade.bid_price() + m_symbol.step() );
            else // order_dir_t::ask
                trade.bid_price( trade.ask_price() - m_symbol.step() );
            trade.start(m_market.datetime());

            STRATEGY_LOG(m_lg, debug) << "should start trade (deal prediction): "
                                      << "bid=" << trade.bid_price() << " "
                                      << "ask=" << trade.ask_price();

            work();
            return true;
        }
    }
    return false;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::cancel(scalper_trade_t& trade)
{
    if (trade.is_canceled())
        return;

    trade.cancel();

    auto& orders = trade.orders;

    for (auto& order : orders)
    {
        if (order.is_for_cancel())
        {
            if (m_market.datetime() - order.datetime() >= seconds(60))
            {
                order.for_cancel(false);
                del_order(order);
            }

            if (order.dir() == order_dir_t::bid)
            {
                if (m_last_sell_tick - order.price() >= 30)
                {
                    order.for_cancel(false);
                    del_order(order);
                }
            }
            else
            {
                if (order.price() - m_last_buy_tick >= 30)
                {
                    order.for_cancel(false);
                    del_order(order);
                }
            }
        }
        else
        {
            del_order(order);
        }
    }

    if (orders.size() > 0)
        return;

    if (trade.sell_volume() != trade.buy_volume())
    {
        order_dir_t dir;
        double price;
        if (trade.buy_volume() > trade.sell_volume())
        {
            dir = order_dir_t::ask;
            price = m_orderbook.best_ask() - m_symbol.step();
        }
        else
        {
            dir = order_dir_t::bid;
            price = m_orderbook.best_bid() + m_symbol.step();
        }
        int volume = std::abs( trade.sell_volume() - trade.buy_volume() );
        scalper_order_t order{m_symbol, dir, price, volume};
        order.for_cancel(true);
        add_order(trade, order);
    }
    else
    {
        trade.set_canceled();
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

/*
10.00 - 14.00 	Начало основной торговой сессии
14.00 - 14.03 	Промежуточный клиринговый сеанс (дневной клиринг)
14.03 - 18.45 	Окончание основной торговой сессии
18.45 - 19.00 	Вечерний клиринговый сеанс
19.00 - 23.50 	Вечерняя торговая сессия
*/

scalper_strategy_3_t::strategy_state_t scalper_strategy_3_t::get_state()
{
    ptime today{m_market.datetime().date(),
                time_duration{0,0,0}};
    ptime time1 = today + time_duration{10,00,01};
    ptime time2 = today + time_duration{13,57,00};
    ptime time3 = today + time_duration{14,00,00};
    ptime time4 = today + time_duration{14,03,01};
    ptime time5 = today + time_duration{18,42,00};
    ptime time6 = today + time_duration{18,45,00};
    ptime time7 = today + time_duration{19,00,01};
    ptime time8 = today + time_duration{23,42,00};
    ptime time9 = today + time_duration{23,50,00};

    ptime now = m_market.datetime();

    if (now < time1)
        return strategy_state_t::stop;

    if ((now >= time1) && (now < time2))
        return strategy_state_t::work;

    if ((now >= time2) && (now < time3))
        return strategy_state_t::prepare_to_stop;

    if ((now >= time3) && (now < time4))
        return strategy_state_t::stop;

    if ((now >= time4) && (now < time5))
        return strategy_state_t::work;

    if ((now >= time5) && (now < time6))
        return strategy_state_t::prepare_to_stop;

    if ((now >= time6) && (now < time7))
        return strategy_state_t::stop;

    if ((now >= time7) && (now < time8))
        return strategy_state_t::work;

    if ((now >= time8) && (now < time9))
        return strategy_state_t::prepare_to_stop;

    if (now >= time9)
        return strategy_state_t::stop;

    return strategy_state_t::stop;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::work()
{
//    for_each_if(m_trades.begin(),
//                m_trades.end(),
//                [](const scalper_trade_t& trade) {
//                    return trade.is_canceled() || trade.is_complete();
//                },
//                [](scalper_trade_t& trade) {

//                    boost::filesystem::path dir("trades");
//                    if(!(boost::filesystem::exists(dir)))
//                        boost::filesystem::create_directory(dir);

//                    if (trade.total() > 0)
//                        trade.event_writer().save_to_file(string{"trades/"} + std::to_string(trade.id()) +
//                                                          "_pos_" + std::to_string(trade.total()) +
//                                                          ".trade");
//                    else if (trade.total() < 0)
//                        trade.event_writer().save_to_file(string{"trades/"} + std::to_string(trade.id()) +
//                                                          "_neg_" + std::to_string(std::abs(trade.total())) +
//                                                          ".trade");
//                });

        for_each_if(m_trades.begin(),
                    m_trades.end(),
                    [](const scalper_trade_t& trade) {
                        return trade.is_canceled() || trade.is_complete();
                    },
                    [this](scalper_trade_t& trade) {
                        stat_t stat;
                        stat.m_first_order = trade.first_order();
                        stat.m_sell_volume_5s = trade.sell_volume_5s();
                        stat.m_buy_volume_5s = trade.buy_volume_5s();
                        stat.m_sell_ticks_5s = trade.sell_ticks_5s();
                        stat.m_buy_ticks_5s = trade.buy_ticks_5s();
                        stat.m_bid_volume_5s = trade.bid_volume_5s();
                        stat.m_bid_volume_100ms = trade.bid_volume_100ms();
                        stat.m_ask_volume_5s = trade.ask_volume_5s();
                        stat.m_ask_volume_100ms = trade.ask_volume_100ms();
                        stat.m_profit = trade.total();
                        m_stat.push_back(stat);
                    });

    m_trades.erase(std::remove_if(m_trades.begin(),
                                  m_trades.end(),
                                  [](const scalper_trade_t& trade) {
                                      return trade.is_canceled() || trade.is_complete();
                                  }),
                   m_trades.end());

    verify_orders();

    while (m_trades.size() < m_trades_amount)
    {
        m_trades.emplace_back( m_symbol );
        init( m_trades.back() );
    }

    for (scalper_trade_t& trade : m_trades)
    {
        if ( !trade.should_cancel() )
        {

            if ( !trade.is_started() )
            {
                if (trade.is_ready())
                    try_to_start(trade);
            }
            else
            {
                for (scalper_order_t& order : trade.orders)
                {
                    if (order.dir() == order_dir_t::bid)
                    {
                        if (order.price() != trade.bid_price())
                            move_order(order, trade.bid_price());
                    }
                    else if (order.dir() == order_dir_t::ask)
                    {
                        if (order.price() != trade.ask_price())
                            move_order(order, trade.ask_price());
                    }
                }

                if (m_market.datetime() - trade.start_time() > seconds(90))
                {
                    STRATEGY_LOG(m_lg, info) << "should cancel trade: "
                                             << "datetime() - trade.start_time() > seconds(30)";
                    trade.cancel();
                }

                if (trade.cur_profit(m_last_buy_tick, m_last_sell_tick) <= -40)
                    trade.cancel();
            }

        }
        else
            cancel(trade);
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::prepare_to_stop()
{
    m_trades.erase(std::remove_if(m_trades.begin(),
                                  m_trades.end(),
                                  [](const scalper_trade_t& trade) {
                                      return trade.is_canceled() || trade.is_complete();
                                  }),
                   m_trades.end());

    verify_orders();

    for (scalper_trade_t& trade : m_trades)
        cancel(trade);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::add_order(scalper_trade_t& trade,
                                   const scalper_order_t& _order)
{
    scalper_order_t order{_order};
    order.datetime(m_market.datetime());
    order.start_pending(m_market.datetime());
    trade.orders.push_back(order);
    strategy_base_t::add_order(order);
    STRATEGY_LOG(m_lg, info) << "CMD: add order id=" << order.id() << " "
                             << dir_str[static_cast<int>(order.dir())]
                             << " price=" << order.price()
                             << " volume=" << order.volume();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bool scalper_strategy_3_t::del_order(scalper_order_t& order)
{
    if (order.is_pending())
        return false;
    order.start_pending(m_market.datetime());
    strategy_base_t::del_order(order);
    STRATEGY_LOG(m_lg, info) << "CMD: del order id=" << order.id();
    return true;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bool scalper_strategy_3_t::move_order(scalper_order_t& order, double price)
{
    if (order.price() == price)
        return true;
    if (order.is_pending())
        return false;
    order.start_pending(m_market.datetime());
    strategy_base_t::move_order(order, price);
    STRATEGY_LOG(m_lg, info) << "CMD: move order id=" << order.id() << " "
                             << dir_str[static_cast<int>(order.dir())]
                             << " to price=" << price;
    return true;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

tuple<int, int> scalper_strategy_3_t::find_order(int order_id)
{
    unsigned int trade_index;
    unsigned int order_index;
    for (trade_index = 0; trade_index < m_trades.size(); trade_index++)
    {
        auto& trade = m_trades[trade_index];
        auto& orders = trade.orders;
        for (order_index = 0; order_index < orders.size(); order_index++)
            if ( orders[order_index].id() == order_id )
                return make_tuple(trade_index, order_index);
    }
    throw -1;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::verify_orders()
{
    for (auto& trade : m_trades)
    {
        auto& orders = trade.orders;
        for (auto& order : orders)
        {
            if (order.is_pending() &&
                (m_market.datetime() - order.start_pending_time() > seconds(30)))
            {
                STRATEGY_LOG(m_lg, debug) << "error: order pending id=" << order.id();
            }
        }
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int scalper_strategy_3_t::orders_before(const scalper_order_t &order, int n)
{
    if (order.dir() == order_dir_t::bid)
    {
        int orders_before = order.orders_before();
        double price = order.price()+m_symbol.step();
        for(int i = 1;
            (i <= n) && (price <= m_orderbook.best_bid());
            i++, price += m_symbol.step())
        {
            orders_before += m_orderbook[price];
        }
        return orders_before;
    }
    else // order_dir_t::ask
    {
        int orders_before = order.orders_before();
        double price = order.price()-m_symbol.step();
        for(int i = 1;
            (i <= n) && (price >= m_orderbook.best_ask());
            i++, price -= m_symbol.step())
        {
            orders_before += m_orderbook[price];
        }
        return orders_before;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int scalper_strategy_3_t::levels_before(const scalper_order_t &order)
{
    if (order.dir() == order_dir_t::bid)
    {
        return static_cast<int>((m_orderbook.best_bid()-order.price())/m_symbol.step());
    }
    else // order_dir_t::ask
    {
        return static_cast<int>((m_orderbook.best_ask()-order.price())/m_symbol.step());
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int scalper_strategy_3_t::orders_after(const scalper_order_t &order, int n)
{
    if (order.dir() == order_dir_t::bid)
    {
        double orders_after = m_orderbook[order.price()] - order.orders_before();
        double price = order.price()-m_symbol.step();
        for(int i = 1; i <= n; i++, price -= m_symbol.step())
        {
            orders_after += m_orderbook[price];
        }
        return orders_after;
    }
    else // order_dir_t::ask
    {
        double orders_after = m_orderbook[order.price()] - order.orders_before();
        double price = order.price()+m_symbol.step();
        for(int i = 1; i <= n; i++, price += m_symbol.step())
        {
            orders_after += m_orderbook[price];
        }
        return orders_after;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int scalper_strategy_3_t::attack_orders(const scalper_order_t &order, int n)
{
    if (order.dir() == order_dir_t::bid)
    {
        double attack_orders = 0;
        double price = order.price()+m_symbol.step();
        for(int i = 1; i <= n; i++, price += m_symbol.step())
        {
            if (price >= m_orderbook.best_ask())
                attack_orders += m_orderbook[price];
        }
        return attack_orders;
    }
    else // order_dir_t::ask
    {
        double attack_orders = 0;
        double price = order.price()-m_symbol.step();
        for(int i = 1; i <= n; i++, price -= m_symbol.step())
        {
            if (price <= m_orderbook.best_bid())
                attack_orders += m_orderbook[price];
        }
        return attack_orders;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int scalper_strategy_3_t::orderbook_volume(order_dir_t dir) const
{
    if (dir == order_dir_t::bid)
    {
        auto bids = m_orderbook.get_bids();
        int volume = 0;
        for (auto it=bids.begin(); it!=bids.end(); it++)
            volume += it->second;
        return volume;
    }
    else
    {
        auto asks = m_orderbook.get_asks();
        int volume = 0;
        for (auto it=asks.begin(); it!=asks.end(); it++)
            volume += it->second;
        return volume;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int scalper_strategy_3_t::tick_volume(deal_type_t type, const time_duration& duration)
{
    int volume = 0;
    for (auto it=m_all_tick_events.rbegin(); it!=m_all_tick_events.rend(); it++)
    {
        if (it->datetime < m_market.datetime() - duration)
            break;
        if (it->tick_type == type)
            volume += it->volume;
    }
    return volume;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::check_logger()
{
    ptime now = m_market.datetime();

    date today = now.date();

    if (today > m_prev_date)
    {
        boost::shared_ptr< logging::core > core = logging::core::get();

        core->remove_sink(m_sink);

        boost::shared_ptr< sinks::text_ostream_backend >
            backend = boost::make_shared< sinks::text_ostream_backend >();

        boost::filesystem::path dir("logs");
        if(!(boost::filesystem::exists(dir)))
            boost::filesystem::create_directory(dir);

        using boost::gregorian::to_iso_extended_string;

        backend->add_stream(
                    boost::shared_ptr< std::ostream >(
                        new std::ofstream(
                            string{"logs/"} + m_symbol.name() +
                                string{"_"}+to_iso_extended_string(today) +
                                string{".log"}))); //, std::ofstream::app)));
        backend->auto_flush(true);

        m_sink = boost::make_shared<sink_t>(backend);

//        boost::shared_ptr< std::ostream > stream(&std::clog, boost::empty_deleter());
//        m_sink->locked_backend()->add_stream(stream);

        m_sink->set_filter(
                    expr::attr<scalper_strategy_3_t*>("pid") == this );

        m_sink->set_formatter(
            expr::format("%1%: %2%")
                % expr::format_date_time<ptime>("time", "%H:%M:%S.%f")
                % expr::smessage
        );

        core->add_sink(m_sink);
    }

    m_time_attr.set(now);

    m_prev_date = today;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_3_t::write_stat() const
{
//    struct stat_t
//    {
//        order_dir_t m_first_order;
//        int m_sell_volume_5s = 0;
//        int m_buy_volume_5s = 0;
//        int m_sell_ticks_5s = 0;
//        int m_buy_ticks_5s = 0;
//        int m_bid_volume_5s = 0;
//        int m_bid_volume_100ms = 0;
//        int m_ask_volume_5s = 0;
//        int m_ask_volume_100ms = 0;
//        double m_profit;
//    };

    std::ofstream file{"stata.csv"};
    //file << "P;V5S;OV5S;T5S;OT5S;O5S;O100MS;OO5S;OO100MS\n";
    for (const stat_t& stat : m_stat)
    {
        if (stat.m_profit == 0)
            continue;
        file << static_cast<int>(stat.m_first_order) << ";";
        file << stat.m_profit << ";";
        if (stat.m_first_order == order_dir_t::bid)
            file << stat.m_sell_volume_5s << ";"
                 << stat.m_buy_volume_5s << ";"
                 << stat.m_sell_ticks_5s << ";"
                 << stat.m_buy_ticks_5s << ";"
                 << stat.m_bid_volume_5s << ";"
                 << stat.m_bid_volume_100ms << ";"
                 << stat.m_ask_volume_5s << ";"
                 << stat.m_ask_volume_100ms << "\n";
        else
            file << stat.m_buy_volume_5s << ";"
                 << stat.m_sell_volume_5s << ";"
                 << stat.m_buy_ticks_5s << ";"
                 << stat.m_sell_ticks_5s << ";"
                 << stat.m_ask_volume_5s << ";"
                 << stat.m_ask_volume_100ms << ";"
                 << stat.m_bid_volume_5s << ";"
                 << stat.m_bid_volume_100ms << "\n";
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
