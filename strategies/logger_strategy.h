#ifndef LOGGER_STRATEGY_HPP
#define LOGGER_STRATEGY_HPP

#include "scalper_headers.h"
#include "../headers.h"
#include "../strategy_base_t.h"
#include "scalper_trade_t.h"

#include "../events/orderbook_event_t.hpp"
#include "../events/tick_event_t.hpp"

#include <fstream>
#include <boost/filesystem.hpp>

namespace strategies
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class logger_strategy_t : public strategy_base_t
{
public:
    logger_strategy_t(
            market_base_t& market,
            symbol_base_t& symbol,
            marketdata_base_t& marketdata);
    ~logger_strategy_t();

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

    std::ofstream file;

    void log(const string& str);

    enum class strategy_state_t { work, stop, prepare_to_stop };

    strategy_state_t m_prev_state = strategy_state_t::stop;

    strategy_state_t get_state();

    void work();
    void prepare_to_stop();
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif
