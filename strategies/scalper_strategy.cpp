#include "scalper_strategy.h"
#include "scalper_order_t.h"
#include "../market_base_t.h"
#include "../deal_base_t.h"
#include "../utils.h"
#include "../orderbook_base_t.h"
#include "../symbol_base_t.h"
#include "../orderlog_base_t.h"
#include "../order_t.h"
#include "../types.h"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/utility/empty_deleter.hpp>

#include <iostream>

namespace strategies
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

scalper_strategy_t::scalper_strategy_t(
        market_base_t& market,
        symbol_base_t& symbol,
        marketdata_base_t& marketdata) :

    strategy_base_t{market, symbol, marketdata}

{
    m_lg.add_attribute("pid", attr::constant<scalper_strategy_t*>(this));
    m_lg.add_attribute("time", m_time_attr);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

scalper_strategy_t::~scalper_strategy_t()
{
    m_lg.remove_all_attributes();
    boost::shared_ptr< logging::core > core = logging::core::get();
    core->remove_sink(m_sink);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_t::order_succeeded(const order_base_t& order)
{
    STRATEGY_LOG(m_lg, debug) << "CALLBACK [order_succeeded] id=" << order.id();

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

    added_order.datetime( order.datetime() );
    added_order.stop_pending();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_t::order_failed(const order_base_t& order)
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

void scalper_strategy_t::order_del_succeeded(const order_base_t& order)
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

    orders.erase( orders.begin()+order_index );
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_t::order_del_failed(const order_base_t& order)
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

void scalper_strategy_t::order_move_succeeded(const order_base_t& order)
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

    moved_order.stop_pending();
    moved_order.price( order.price() );
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_t::order_move_failed(const order_base_t& order)
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

void scalper_strategy_t::add_deal(const deal_base_t& deal)
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
    }

    order.volume( order.volume() - deal.volume() );
    if ( order.volume() <= 0 )
        orders.erase( orders.begin()+order_index );

    trade.add_deal(deal);

    if ((trade.buy_volume() == trade.sell_volume()) && (trade.orders.size() == 0))
    {
        STRATEGY_LOG(m_lg, info) << "trade complete " << m_tradelog.total();
        trade.complete();
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_t::add_tick(const deal_base_t& tick)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_t::process()
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

void scalper_strategy_t::init(scalper_trade_t& trade)
{
    STRATEGY_LOG(m_lg, info) << "CMD: trade init";

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
    double price = quotes[0].price;
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
    price = quotes[0].price;
    trade.ask_price(price);
    add_order(trade, scalper_order_t{m_symbol, order_dir_t::ask, price, 1});
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bool scalper_strategy_t::try_to_start(scalper_trade_t& trade)
{
    for (scalper_order_t& order : trade.orders)
    {
        bool should_start = false;

        int orders_before = 0;
        int orders_after = 0;

        if (order.dir() == order_dir_t::bid)
        {
            auto bids = m_orderlog.bids();
            order_t* order_ptr = nullptr;
            for (size_t i = 0; i < bids.size(); i++)
                if ( order.id() == bids[i].id() )
                    order_ptr = &bids[i];
            if (order_ptr == nullptr)
                return false;
            orders_before = order_ptr->orders_before();
            for (double price = order.price()+m_symbol.step();
                 price <= m_orderbook.best_bid();
                 price += m_symbol.step())
            {
                orders_before += m_orderbook[price];
            }
            orders_after = m_orderbook[order.price()] -
                               order_ptr->orders_before() -
                               order_ptr->volume();
            orders_after += m_orderbook[order.price() - m_symbol.step()];
        }
        else // order_dir_t::ask
        {
            auto asks = m_orderlog.asks();
            order_t* order_ptr = nullptr;
            for (size_t i = 0; i < asks.size(); i++)
                if ( order.id() == asks[i].id() )
                    order_ptr = &asks[i];
            if (order_ptr == nullptr)
                return false;
            orders_before = order_ptr->orders_before();
            for (double price = order.price()-m_symbol.step();
                 price >= m_orderbook.best_ask();
                 price -= m_symbol.step())
            {
                orders_before += m_orderbook[price];
            }
            orders_after = m_orderbook[order.price()] -
                               order_ptr->orders_before() -
                               order_ptr->volume();
            orders_after += m_orderbook[order.price() + m_symbol.step()];
        }

        if ((orders_after <= 20) && (orders_before <= 20))
            should_start = true;

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
            return true;
        }
    }
    return false;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_t::cancel(scalper_trade_t& trade)
{
    if (trade.is_canceled())
        return;

    trade.cancel();

    auto& orders = trade.orders;

    for (auto& order : orders)
    {
        if (order.is_for_cancel())
        {
            if (m_market.datetime() - order.datetime() >= seconds(20))
            {
                order.for_cancel(false);
                del_order(order);
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

scalper_strategy_t::strategy_state_t scalper_strategy_t::get_state()
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

void scalper_strategy_t::work()
{
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
                try_to_start(trade);
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

                if (m_market.datetime() - trade.start_time() > seconds(60))
                {
                    STRATEGY_LOG(m_lg, info) << "should cancel trade: "
                                             << "datetime() - trade.start_time() > seconds(60)";
                    trade.cancel();
                }
            }

        }
        else
            cancel(trade);
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_t::prepare_to_stop()
{
    m_trades.erase(std::remove_if(m_trades.begin(),
                                  m_trades.end(),
                                  [](const scalper_trade_t& trade) {
                                      return trade.is_canceled();
                                  }),
                   m_trades.end());

    verify_orders();

    for (scalper_trade_t& trade : m_trades)
        cancel(trade);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_strategy_t::add_order(scalper_trade_t& trade,
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

bool scalper_strategy_t::del_order(scalper_order_t& order)
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

bool scalper_strategy_t::move_order(scalper_order_t& order, double price)
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

tuple<int, int> scalper_strategy_t::find_order(int order_id)
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

void scalper_strategy_t::verify_orders()
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

void scalper_strategy_t::check_logger()
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
                    expr::attr<scalper_strategy_t*>("pid") == this );

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

}
