#ifndef CARCASS_STRATEGY_T_H
#define CARCASS_STRATEGY_T_H

#include "../headers.h"
#include "../strategy_base_t.h"

namespace strategies
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

using namespace trading;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class carcass_strategy_t : public strategy_base_t
{
public:

    carcass_strategy_t(
            market_base_t& market,
            symbol_base_t& symbol,
            marketdata_base_t& marketdata);

    void order_succeeded(const order_base_t& order) override;
    void order_failed(const order_base_t& order) override;

    void order_del_succeeded(const order_base_t& order) override;
    void order_del_failed(const order_base_t& order) override;

    void order_move_succeeded(const order_base_t& order) override;
    void order_move_failed(const order_base_t& order) override;

    void add_deal(const deal_base_t& deal) override;
    void add_tick(const deal_base_t& tick) override;

    void process() override;

    //size_t proc_counter = 0;

private:

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CARCASS_STRATEGY_T_H
