#include "logger_strategy.h"
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

logger_strategy_t::logger_strategy_t(
        market_base_t& market,
        symbol_base_t& symbol,
        marketdata_base_t& marketdata) :

    strategy_base_t{market, symbol, marketdata}

{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

logger_strategy_t::~logger_strategy_t()
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::order_succeeded(const order_base_t& order)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::order_failed(const order_base_t& order)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::order_del_succeeded(const order_base_t& order)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::order_del_failed(const order_base_t& order)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::order_move_succeeded(const order_base_t& order)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::order_move_failed(const order_base_t& order)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::add_deal(const deal_base_t& deal)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::add_tick(const deal_base_t& tick)
{
    using boost::posix_time::time_facet;
    time_facet *facet = new time_facet(datetime_format);
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << "t;" << m_market.datetime() << ";" << tick.price() << ",";
    if (tick.type() == deal_type_t::buy)
        ss << -tick.volume();
    else
        ss << tick.volume();
    log(ss.str());
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::process()
{
    const strategy_state_t cur_state = get_state();

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


void logger_strategy_t::update_orders_before()
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::update_orderbook()
{
    using boost::posix_time::time_facet;
    time_facet *facet = new time_facet(datetime_format);
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << "s;" << m_market.datetime() << ";";
    auto asks = m_orderbook.get_asks();
    for (auto it = asks.rbegin(); it != asks.rend(); it++)
    {
        if (it != asks.rbegin())
            ss << ",";
        ss << it->first << "," << -(it->second);
    }
    auto bids = m_orderbook.get_bids();
    for (auto it = bids.begin(); it != bids.end(); it++)
    {
        ss << "," << it->first << "," << it->second;
    }
    log(ss.str());
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::log(const string& str)
{
    if (!file.is_open())
    {
        boost::filesystem::path dir("events_logs");
        if(!(boost::filesystem::exists(dir)))
            boost::filesystem::create_directory(dir);

        file.open(string{"events_logs/"} +
                  string{"events_"} + m_symbol.name() + string{"_"} +
                  to_iso_extended_string(m_market.datetime().date()) +
                  string{".log"});
    }

    file << str << "\n";
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

logger_strategy_t::strategy_state_t logger_strategy_t::get_state()
{
    ptime today{m_market.datetime().date(),
                time_duration{0,0,0}};
    //ptime time1 = today + time_duration{10,00,01};
    ptime time1 = today + time_duration{10,5,1};
    ptime time2 = today + time_duration{13,57,0};
    ptime time3 = today + time_duration{14,0,0};
    //ptime time4 = today + time_duration{14,03,01};
    ptime time4 = today + time_duration{14,8,01};
    ptime time5 = today + time_duration{18,42,0};
    ptime time6 = today + time_duration{18,45,0};
    //ptime time7 = today + time_duration{19,00,01};
    ptime time7 = today + time_duration{19,5,1};
    ptime time8 = today + time_duration{23,42,0};
    ptime time9 = today + time_duration{23,50,0};

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

void logger_strategy_t::work()
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void logger_strategy_t::prepare_to_stop()
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
