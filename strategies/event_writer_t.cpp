#include "event_writer_t.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#include "sstream"

#include "../utils.h"

#include <iostream>

namespace strategies
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

event_writer_t::event_writer_t(const symbol_base_t& symbol) :
    m_symbol(symbol)
{
}

void event_writer_t::add_stocks_event(const orderbook_event_t& event)
{
    m_stocks_events.push_back(event);
    m_iters.push_back( event_iter_t{ event_iter_type::stocks,
                                     event.datetime,
                                     static_cast<void*>(&m_stocks_events.back()) } );
}

void event_writer_t::add_tick_event(const tick_event_t& event)
{
    m_tick_events.push_back(event);
    m_iters.push_back( event_iter_t{ event_iter_type::tick,
                                     event.datetime,
                                     static_cast<void*>(&m_tick_events.back()) } );

}

void event_writer_t::add_deal_event(const deal_base_t& event)
{
    m_deal_events.push_back(event);
    m_iters.push_back( event_iter_t{ event_iter_type::deal,
                                     event.datetime(),
                                     static_cast<void*>(&m_deal_events.back()) } );
}

void event_writer_t::add_order_event(const scalper_order_t& event)
{
    m_order_events.push_back(event);
    m_iters.push_back( event_iter_t{ event_iter_type::order,
                                     event.datetime(),
                                     static_cast<void*>(&m_order_events.back()) } );
}

void event_writer_t::init_stocks(const decltype(m_stocks_events)& stocks_events)
{
    m_stocks_init = true;
    m_stocks_events = stocks_events;
    for (auto& event : m_stocks_events)
        m_iters.push_back( event_iter_t{ event_iter_type::stocks,
                                         event.datetime,
                                         static_cast<void*>(&event) } );
}

void event_writer_t::init_tick(const decltype(m_tick_events)& tick_events)
{
    m_tick_init = true;
    m_tick_events = tick_events;
    for (auto& event : m_tick_events)
        m_iters.push_back( event_iter_t{ event_iter_type::tick,
                                         event.datetime,
                                         static_cast<void*>(&event) } );
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void event_writer_t::save_to_file(const string& file_name)
{
    std::ofstream file{file_name};

    file << "step;" << std::to_string(m_symbol.step()) << "\n";

    std::stable_sort(m_iters.begin(), m_iters.end(),
                     [](const event_iter_t& iter1, const event_iter_t& iter2)
                     { return iter1.datetime < iter2.datetime; });


    for (auto& iter : m_iters)
    {
        switch (iter.type)
        {
        case event_iter_type::stocks:
            save_orderbook(file, *static_cast<orderbook_event_t*>(iter.data));
            break;

        case event_iter_type::tick:
            save_tick(file, *static_cast<tick_event_t*>(iter.data));
            break;

        case event_iter_type::deal:
            save_deal(file, *static_cast<deal_base_t*>(iter.data));
            break;

        case event_iter_type::order:
            save_order(file, *static_cast<scalper_order_t*>(iter.data));
            break;

        default:
            break;
        }
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int event_writer_t::sell_volume_5s() const
{
    ptime current_datetime = m_iters.back().datetime;

    using boost::posix_time::to_simple_string;
//    std::cout << to_iso_string(current_datetime-seconds(5)) << std::endl;
//    std::cout << to_iso_string(current_datetime-millisec(100)) << std::endl;
//    std::cout << "***************************" << std::endl;

    int volume = 0;

    //std::cout << "#:";
    std::for_each(m_tick_events.begin(),
                  m_tick_events.end(),
                  [&volume, &current_datetime](const tick_event_t& tick_event) mutable
                      {
                          if ((tick_event.datetime >= current_datetime-seconds(5)) && (tick_event.datetime <= current_datetime-millisec(100)))
                          if (tick_event.tick_type == deal_type_t::sell)
                          {
                              //std::cout << tick_event.volume << "(" << to_simple_string(tick_event.datetime) << "),";
                              volume += tick_event.volume;
                          }
                      }
                  );

    //std::cout << volume << std::endl;

    return volume;
}

int event_writer_t::buy_volume_5s() const
{
    ptime current_datetime = m_iters.back().datetime;

//    using boost::posix_time::to_iso_string;
//    std::cout << to_iso_string(current_datetime-seconds(5)) << std::endl;
//    std::cout << to_iso_string(current_datetime-millisec(100)) << std::endl;
//    std::cout << "***************************" << std::endl;

    int volume = 0;

    //std::cout << "#:";
    std::for_each(m_tick_events.begin(),
                  m_tick_events.end(),
                  [&volume, &current_datetime](const tick_event_t& tick_event) mutable
                      {
                          if ((tick_event.datetime >= current_datetime-seconds(5)) && (tick_event.datetime <= current_datetime-millisec(100)))
                          if (tick_event.tick_type == deal_type_t::buy)
                          {
                              //std::cout << tick_event.volume << "(" << to_simple_string(tick_event.datetime) << "),";
                              volume += tick_event.volume;
                          }
                      }
                  );

    //std::cout << volume << std::endl;
    //std::cout << "***************************" << std::endl;

    return volume;
}

int event_writer_t::sell_ticks_5s() const
{
    ptime current_datetime = m_iters.back().datetime;

    int ticks = 0;

    std::for_each(m_tick_events.begin(),
                  m_tick_events.end(),
                  [&](const tick_event_t& tick_event) mutable
                      {
                          if ((tick_event.datetime >= current_datetime-seconds(5)) && (tick_event.datetime <= current_datetime-millisec(100)))
                          if (tick_event.tick_type == deal_type_t::sell)
                              ticks++;
                      }
                  );

    return ticks;
}

int event_writer_t::buy_ticks_5s() const
{
    ptime current_datetime = m_iters.back().datetime;

    int ticks = 0;

    std::for_each(m_tick_events.begin(),
                  m_tick_events.end(),
                  [&](const tick_event_t& tick_event) mutable
                      {
                          if ((tick_event.datetime >= current_datetime-seconds(5)) && (tick_event.datetime <= current_datetime-millisec(100)))
                          if (tick_event.tick_type == deal_type_t::buy)
                              ticks++;
                      }
                  );

    return ticks;
}

int event_writer_t::bid_volume_5s() const
{
    ptime current_datetime = m_iters.back().datetime;
    auto it = std::find_if(m_stocks_events.begin(),
                           m_stocks_events.end(),
                           [&](const orderbook_event_t& stock_event)
                           { return stock_event.datetime >= current_datetime-seconds(5); });
    if (it == m_stocks_events.end())
        return 0;
    const orderbook_event_t& stock_event = *it;
    int volume = 0;
    int i = 0;
    for (auto it = stock_event.bids.begin(); (it!=stock_event.bids.end()) && (i < 10); it++, i++)
    {
        volume += it->second;
    }
    return volume;
}

int event_writer_t::bid_volume_100ms() const
{
    ptime current_datetime = m_iters.back().datetime;
    auto it = std::find_if(m_stocks_events.rbegin(),
                           m_stocks_events.rend(),
                           [&](const orderbook_event_t& stock_event)
                           { return stock_event.datetime <= current_datetime-millisec(100); });
    if (it == m_stocks_events.rend())
        return 0;
    const orderbook_event_t& stock_event = *it;
    int volume = 0;
    int i = 0;
    for (auto it = stock_event.bids.begin(); (it!=stock_event.bids.end()) && (i < 10); it++, i++)
    {
        volume += it->second;
    }
    return volume;
}

int event_writer_t::ask_volume_5s() const
{
    ptime current_datetime = m_iters.back().datetime;
    auto it = std::find_if(m_stocks_events.begin(),
                           m_stocks_events.end(),
                           [&](const orderbook_event_t& stock_event)
                           { return stock_event.datetime >= current_datetime-seconds(5); });
    if (it == m_stocks_events.end())
        return 0;
    const orderbook_event_t& stock_event = *it;
    int volume = 0;
    int i = 0;
    for (auto it = stock_event.asks.begin(); (it!=stock_event.asks.end()) && (i < 10); it++, i++)
    {
        volume += it->second;
    }
    return volume;
}

int event_writer_t::ask_volume_100ms() const
{
    ptime current_datetime = m_iters.back().datetime;
    auto it = std::find_if(m_stocks_events.rbegin(),
                           m_stocks_events.rend(),
                           [&](const orderbook_event_t& stock_event)
                           { return stock_event.datetime <= current_datetime-millisec(100); });
    if (it == m_stocks_events.rend())
        return 0;
    const orderbook_event_t& stock_event = *it;
    int volume = 0;
    int i = 0;
    for (auto it = stock_event.asks.begin(); (it!=stock_event.asks.end()) && (i < 10); it++, i++)
    {
        volume += it->second;
    }
    return volume;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void event_writer_t::save_tick(std::ofstream& stream, const tick_event_t& tick)
{
    using boost::posix_time::time_facet;
    time_facet *facet = new time_facet(time_format);
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << "t;" << tick.datetime << ";" << tick.price << ";";
    if (tick.tick_type == deal_type_t::buy)
        ss << -tick.volume;
    else
        ss << tick.volume;
    ss << "\n";
    stream << ss.rdbuf();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void event_writer_t::save_orderbook(std::ofstream& stream, const orderbook_event_t& orderbook)
{
    using boost::posix_time::time_facet;
    time_facet *facet = new time_facet(time_format);
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << "s;" << orderbook.datetime;
    auto& asks = orderbook.asks;
    for (auto it = asks.rbegin(); it != asks.rend(); it++)
        ss << ";" << it->first << ";" << -(it->second);
    auto& bids = orderbook.bids;
    for (auto it = bids.begin(); it != bids.end(); it++)
        ss << ";" << it->first << ";" << it->second;
    ss << "\n";
    stream << ss.rdbuf();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void event_writer_t::save_deal(std::ofstream& stream, const deal_base_t& deal)
{
    using boost::posix_time::time_facet;
    time_facet *facet = new time_facet(time_format);
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << "d;" << deal.datetime() << ";" << deal.price() << ";";
    if (deal.type() == deal_type_t::buy)
        ss << -deal.volume();
    else
        ss << deal.volume();
    ss << "\n";
    stream << ss.rdbuf();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void event_writer_t::save_order(std::ofstream &stream, const scalper_order_t& order)
{
    using boost::posix_time::time_facet;
    time_facet *facet = new time_facet(time_format);
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), facet));
    ss << "o;" << order.datetime() << ";"
       << order.price() << ";";
    if (order.dir() == order_dir_t::bid)
        ss << order.volume();
    else
        ss << -order.volume();
    ss << ";" << order.orders_before() <<"\n";
    stream << ss.rdbuf();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
