#ifndef SCALPER_TRADE_T_H
#define SCALPER_TRADE_T_H

#include "scalper_headers.h"
#include "../headers.h"
#include "scalper_order_t.h"
#include "event_writer_t.h"

namespace strategies
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class scalper_trade_t
{
public:
    scalper_trade_t(const symbol_base_t& symbol) :
        m_id{++s_counter},
        m_symbol(symbol)
    {
    }

    scalper_trade_t& operator= (const scalper_trade_t& trade)
    {
        if (this == &trade)
            return *this;

        m_id = trade.m_id;
        orders = trade.orders;
        m_event_writer = trade.m_event_writer;

        m_start_time = trade.m_start_time;
        m_is_started = trade.m_is_started;
        m_should_cancel = trade.m_should_cancel;
        m_is_canceled = trade.m_is_canceled;
        m_is_complete = trade.m_is_complete;
        m_is_ready = trade.m_is_ready;

        m_ask_price = trade.m_ask_price;
        m_bid_price = trade.m_bid_price;

        m_sell_volume = trade.m_sell_volume;
        m_buy_volume = trade.m_buy_volume;

        m_buy_total = trade.m_buy_total;
        m_sell_total = trade.m_sell_total;

        return *this;
    }

    vector<scalper_order_t> orders;

    int id() const { return m_id; }

    void add_deal(const deal_base_t& deal);

    void start(const ptime& start_time)
    {
        m_is_started = true;
        m_start_time = start_time;
    }
    bool is_started() const { return m_is_started; }
    ptime start_time() const { return m_start_time; }

    bool should_cancel() const { return m_should_cancel; }
    void cancel() { m_should_cancel = true; }

    void set_canceled() { m_is_canceled = true; }
    bool is_canceled() const { return m_is_canceled; }

    double bid_price() const { return m_bid_price; }
    void bid_price(double bid_price) { m_bid_price = bid_price; }

    double ask_price() const { return m_ask_price; }
    void ask_price(double ask_price) { m_ask_price = ask_price; }

    bool is_complete() const { return m_is_complete; }
    void complete() { m_is_complete = true; }

    bool is_ready();
    void ready() { m_is_ready = true; }

    double buy_total() const { return m_buy_total; }
    void buy_total(double buy_total) { m_buy_total = buy_total; }

    double sell_total() const { return m_sell_total; }
    void sell_total(double sell_total) { m_sell_total = sell_total; }

    double total() const { return m_sell_total - m_buy_total; }

    int sell_volume() const { return m_sell_volume; }
    void sell_volume(int sell_volume) { m_sell_volume = sell_volume; }

    int buy_volume() const { return m_buy_volume; }
    void buy_volume(int buy_volume) { m_buy_volume = buy_volume; }

    event_writer_t& event_writer() { return m_event_writer; }

    double cur_profit(double best_ask, double best_bid) const;

    void set_first_order(order_dir_t first_order) { m_first_order = first_order; }

    void set_sell_volume_5s() { m_sell_volume_5s = m_event_writer.sell_volume_5s(); }
    void set_buy_volume_5s() { m_buy_volume_5s = m_event_writer.buy_volume_5s(); }
    void set_sell_ticks_5s() { m_sell_ticks_5s = m_event_writer.sell_ticks_5s(); }
    void set_buy_ticks_5s() { m_buy_ticks_5s = m_event_writer.buy_ticks_5s(); }

    void set_bid_volume_5s() { m_bid_volume_5s = m_event_writer.bid_volume_5s(); }
    void set_bid_volume_100ms() { m_bid_volume_100ms = m_event_writer.bid_volume_100ms(); }
    void set_ask_volume_5s() { m_ask_volume_5s = m_event_writer.ask_volume_5s(); }
    void set_ask_volume_100ms() { m_ask_volume_100ms = m_event_writer.ask_volume_100ms(); }

    order_dir_t first_order() const { return m_first_order; }

    int sell_volume_5s() const { return m_sell_volume_5s; }
    int buy_volume_5s() const { return m_buy_volume_5s; }
    int sell_ticks_5s() const { return m_sell_ticks_5s; }
    int buy_ticks_5s() const { return m_buy_ticks_5s; }

    int bid_volume_5s() const { return m_bid_volume_5s; }
    int bid_volume_100ms() const { return m_bid_volume_100ms; }
    int ask_volume_5s() const { return m_ask_volume_5s; }
    int ask_volume_100ms() const { return m_ask_volume_100ms; }

    int distance() const { return m_distance; }
    void reduce_distance() { m_distance--; }

    void create_time(const ptime& create_time) { m_create_time = create_time; }
    ptime create_time() const { return m_create_time; }

    std::function<void()> handle_tick = []() {};

    bool is_first = false;
    double alternate_ask = 0.0;
    int alternate_ask_volume = 0;
    double alternate_bid = 0.0;
    int alternate_bid_volume = 0;
    double check_price = 0.0;
    bool is_alternate = false;
    int bidask;

private:
    int m_id;
    const symbol_base_t& m_symbol;
    event_writer_t m_event_writer{m_symbol};

    ptime m_start_time = ptime{min_date_time};
    bool m_is_started = false;
    bool m_should_cancel = false;
    bool m_is_canceled = false;
    bool m_is_complete = false;
    bool m_is_ready = false;

    int m_distance = 4;
    ptime m_create_time = ptime{min_date_time};

    double m_ask_price = 0.0;
    double m_bid_price = 0.0;

    int m_sell_volume = 0;
    int m_buy_volume = 0;

    double m_buy_total = 0.0;
    double m_sell_total = 0.0;

    order_dir_t m_first_order;
    int m_sell_volume_5s = 0;
    int m_buy_volume_5s = 0;
    int m_sell_ticks_5s = 0;
    int m_buy_ticks_5s = 0;
    int m_bid_volume_5s = 0;
    int m_bid_volume_100ms = 0;
    int m_ask_volume_5s = 0;
    int m_ask_volume_100ms = 0;

    static int s_counter;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // SCALPER_TRADE_T_H
