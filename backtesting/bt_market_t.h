#ifndef BT_MARKET_T_H
#define BT_MARKET_T_H

#include "bt_headers.h"
#include "../market_base_t.h"
#include "bt_robot_t.h"
#include "../events/orderbook_event_t.hpp"
#include "../events/tick_event_t.hpp"
#include "bt_task_t.h"

#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/attributes.hpp>

#define BT_LOG() \
    m_time_attr.set(m_datetime); \
    BOOST_LOG(m_lg)

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class bt_market_t : public trading::market_base_t
{
public:
    bt_market_t(const string& filename);

    trading::robot_base_t& robot() override;

    void run() override;

    void add_task(const ptime& datetime, const function<void()>& task);

    void add_order(const bt_order_t& order, bool call_callback=true);
    void del_order(const bt_order_t& order, bool call_callback=true);
    void move_order(const bt_order_t& order, double price);
    void add_deal(const bt_deal_t& deal);

    void set_orderbook();
    void set_orderlog();
    void set_deal_log();

    ptime datetime() const override { return m_datetime; }
    millisec delay() const { return m_delay; }
    ptime task_time() const { return m_datetime+m_delay; }

private:
    vector<bt_orderlog_t>  m_orderlogs;
    vector<bt_deal_log_t>  m_deal_logs;
    vector<bt_orderbook_t> m_orderbooks;

    vector<orderbook_event_t> m_orderbook_events;
    vector<tick_event_t> m_tick_events;

    ifstream m_market_events_file;

    queue< vector<bt_task_t> > m_tasks;

    millisec m_delay{100};
    ptime m_datetime{min_date_time};
    bt_robot_t m_robot;

    void init_strategy(trading::strategy_base_t* strategy) override;
    market_event_t& get_market_event();
    void handle_orderbook_event(const orderbook_event_t& orderbook_event);
    void handle_tick_event(const tick_event_t& tick_event);
    int get_position(const bt_symbol_t& symbol);
    int get_position(const string& symbol_name);

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

    boost::log::sources::logger m_lg;
    boost::log::attributes::mutable_constant<ptime> m_time_attr{ ptime{min_date_time} };
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_MARKET_T_H
