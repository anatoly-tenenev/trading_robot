#ifndef SCALPER_STRATEGY_HPP
#define SCALPER_STRATEGY_HPP

#include "scalper_headers.h"
#include "../headers.h"
#include "../strategy_base_t.h"
#include "scalper_trade_t.h"

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

class scalper_strategy_t : public strategy_base_t
{
public:
    enum log_level
    {
        info,
        debug
    };

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

    scalper_strategy_t(
            market_base_t& market,
            symbol_base_t& symbol,
            marketdata_base_t& marketdata);
    ~scalper_strategy_t();

    void order_succeeded(const order_base_t& order) override;
    void order_failed(const order_base_t& order) override;

    void order_del_succeeded(const order_base_t& order) override;
    void order_del_failed(const order_base_t& order) override;

    void order_move_succeeded(const order_base_t& order) override;
    void order_move_failed(const order_base_t& order) override;

    void add_deal(const deal_base_t& deal) override;
    void add_tick(const deal_base_t& tick) override;

    void process() override;

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

protected:

    enum class strategy_state_t { work, stop, prepare_to_stop };

    strategy_state_t m_prev_state = strategy_state_t::stop;

    vector<scalper_trade_t> m_trades;
    unsigned int m_trades_amount = 1;

    src::severity_logger<log_level> m_lg;
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > sink_t;
    boost::shared_ptr< sink_t > m_sink;

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

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

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

private:
    date m_prev_date = date{min_date_time};
    attr::mutable_constant<ptime> m_time_attr{ ptime{min_date_time} };

    void check_logger();
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // SCALPER_STRATEGY_HPP
