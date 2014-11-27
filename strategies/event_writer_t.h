#ifndef EVENT_WRITER_T_H
#define EVENT_WRITER_T_H

#include "scalper_headers.h"

#include "../events/orderbook_event_t.hpp"
#include "../events/tick_event_t.hpp"
#include "../deal_base_t.h"
#include "scalper_order_t.h"

#include <fstream>

namespace strategies
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class event_writer_t
{
private:
    deque<orderbook_event_t> m_stocks_events;
    deque<tick_event_t> m_tick_events;
    deque<deal_base_t> m_deal_events;
    deque<scalper_order_t> m_order_events;

public:
    event_writer_t(const symbol_base_t& symbol);

    event_writer_t& operator= (const event_writer_t& event_writer)
    {
        if (this == &event_writer)
            return *this;

        assert(m_symbol.equal(event_writer.m_symbol));

        m_stocks_events = event_writer.m_stocks_events;
        m_tick_events = event_writer.m_tick_events;
        m_deal_events = event_writer.m_deal_events;
        m_order_events = event_writer.m_order_events;

        m_stocks_init = event_writer.m_stocks_init;
        m_tick_init = event_writer.m_tick_init;

        m_iters = event_writer.m_iters;

        return *this;
    }

    void add_stocks_event(const orderbook_event_t& event);
    void add_tick_event(const tick_event_t& event);
    void add_deal_event(const deal_base_t& event);
    void add_order_event(const scalper_order_t& event);

    bool is_stocks_init() const { return m_stocks_init; }
    bool is_tick_init() const { return m_tick_init; }

    void init_stocks(const decltype(m_stocks_events)& stocks_events);
    void init_tick(const decltype(m_tick_events)& tick_events);

    void save_to_file(const string& file_name);

    int sell_volume_5s() const;
    int buy_volume_5s() const;
    int sell_ticks_5s() const;
    int buy_ticks_5s() const;

    int bid_volume_5s() const;
    int bid_volume_100ms() const;
    int ask_volume_5s() const;
    int ask_volume_100ms() const;

private:
    const symbol_base_t& m_symbol;

    bool m_stocks_init = false;
    bool m_tick_init = false;

    void save_tick(std::ofstream& stream, const tick_event_t& tick);
    void save_orderbook(std::ofstream& stream, const orderbook_event_t& orderbook);
    void save_deal(std::ofstream& stream, const deal_base_t& deal);
    void save_order(std::ofstream& stream, const scalper_order_t& order);

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

    enum class event_iter_type { stocks, tick, deal, order };

    struct event_iter_t
    {
        event_iter_type type;
        ptime datetime;
        void* data;
    };

    vector<event_iter_t> m_iters;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // EVENT_WRITER_T_H
