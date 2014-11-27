#ifndef SCALPER_STRATEGY_5_HPP
#define SCALPER_STRATEGY_5_HPP

#include "scalper_headers.h"
#include "../headers.h"
#include "../strategy_base_t.h"
#include "scalper_trade_t.h"

#include "../events/orderbook_event_t.hpp"
#include "../events/tick_event_t.hpp"

#include <fstream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>

#define STRATEGY_LOG(logger, level) \
    check_logger(), BOOST_LOG_SEV(logger, level)

#include <boost/filesystem.hpp>

namespace strategies
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace attr = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class index_value_t
{
    ptime datetime;
    double price;
};

class scalper_strategy_5_t : public strategy_base_t
{
public:
    enum log_level
    {
        info,
        debug
    };

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

    scalper_strategy_5_t(
            market_base_t& market,
            symbol_base_t& symbol,
            marketdata_base_t& marketdata);
    ~scalper_strategy_5_t();

    void order_succeeded(const order_base_t& order) override;
    void order_failed(const order_base_t& order) override;

    void order_del_succeeded(const order_base_t& order) override;
    void order_del_failed(const order_base_t& order) override;

    void order_move_succeeded(const order_base_t& order) override;
    void order_move_failed(const order_base_t& order) override;

    void add_deal(const deal_base_t& deal) override;
    void add_tick(const deal_base_t& tick) override;

    void process() override;

    void update_orders_before() override;
    void update_orderbook() override;

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

protected:

    enum class strategy_state_t { work, stop, prepare_to_stop };

    strategy_state_t m_prev_state = strategy_state_t::stop;

    vector<scalper_trade_t> m_trades;
    void trade_emplace();
    void process_trades();

    unsigned int m_trades_amount = 1;

    src::severity_logger<log_level> m_lg;
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > sink_t;
    boost::shared_ptr< sink_t > m_sink;

    deque<orderbook_event_t> m_orderbook_events;
    deque<tick_event_t> m_tick_events;
    deque<tick_event_t> m_all_tick_events;

    vector<index_value_t> m_rtsi;

    double m_last_sell_tick = 0.0;
    double m_last_buy_tick = 0.0;

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

    void add_trade(double bid_price, double ask_price, int volume);
    double profit_of(scalper_trade_t& trade);
    void init(scalper_trade_t& trade);
    bool try_to_start(scalper_trade_t& trade);
    void cancel(scalper_trade_t& trade);

    strategy_state_t get_state();

    void work();
    void prepare_to_stop();

    void add_order(scalper_trade_t& trade, const scalper_order_t& order);
    bool del_order(scalper_order_t& order);
    bool move_order(scalper_order_t& order, double price);

    tuple<int, int> find_order(int order_id);
    void verify_orders();

    int orders_before(const scalper_order_t& order, int n);
    int levels_before(const scalper_order_t& order);
    int orders_after(const scalper_order_t& order, int n);
    int attack_orders(const scalper_order_t& order, int n);
    int orderbook_volume(order_dir_t dir, int depth=0) const;
    int tick_volume(deal_type_t type, const time_duration& duration);

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

private:
    date m_prev_date = date{min_date_time};
    attr::mutable_constant<ptime> m_time_attr{ ptime{min_date_time} };

    void check_logger();

    struct stat_t
    {
        order_dir_t m_first_order;
        int m_sell_volume_5s = 0;
        int m_buy_volume_5s = 0;
        int m_sell_ticks_5s = 0;
        int m_buy_ticks_5s = 0;
        int m_bid_volume_5s = 0;
        int m_bid_volume_100ms = 0;
        int m_ask_volume_5s = 0;
        int m_ask_volume_100ms = 0;
        double m_profit;
    };

    vector<stat_t> m_stat;
    void write_stat() const;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // SCALPER_STRATEGY_5_HPP
