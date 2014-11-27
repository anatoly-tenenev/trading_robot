#include "carcass_strategy_t.h"
#include "../deal_base_t.h"
#include <iostream>

namespace strategies
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

carcass_strategy_t::carcass_strategy_t(
        market_base_t& market,
        symbol_base_t& symbol,
        marketdata_base_t& marketdata) :

    strategy_base_t{market, symbol, marketdata}

{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void carcass_strategy_t::order_succeeded(const order_base_t& order)
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void carcass_strategy_t::order_failed(const order_base_t& order)
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void carcass_strategy_t::order_del_succeeded(const order_base_t& order)
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void carcass_strategy_t::order_del_failed(const order_base_t& order)
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void carcass_strategy_t::order_move_succeeded(const order_base_t& order)
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void carcass_strategy_t::order_move_failed(const order_base_t& order)
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void carcass_strategy_t::add_deal(const deal_base_t& deal)
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void carcass_strategy_t::add_tick(const deal_base_t& tick)
{
//    using namespace std;
//    if (tick.type() == deal_type_t::buy)
//        cout << "buy ";
//    else
//        cout << "sell ";
//    cout << tick.price() << " " << tick.volume() << endl;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void carcass_strategy_t::process()
{
    //proc_counter++;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

