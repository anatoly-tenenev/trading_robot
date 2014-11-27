#include "strategy_base_t.h"
#include "market_base_t.h"
#include "marketdata_base_t.h"
#include "robot_base_t.h"
#include "order_base_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

strategy_base_t::strategy_base_t(
        market_base_t& market,
        symbol_base_t& symbol,
        marketdata_base_t& marketdata) :

    m_market(market),
    m_robot(market.robot()),
    m_symbol(symbol),
    m_orderlog(marketdata.orderlog()),
    m_orderbook(marketdata.orderbook()),
    m_deal_log(marketdata.deal_log())

{
    market.add_strategy(this);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

strategy_base_t::~strategy_base_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void strategy_base_t::add_trade(const trade_t &trade)
{
    m_tradelog.add(trade);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void strategy_base_t::update_orders_before()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void strategy_base_t::update_orderbook()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void strategy_base_t::add_order(const order_base_t& order)
{
    m_robot.add_order(order);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void strategy_base_t::del_order(const order_base_t& order)
{
    m_robot.del_order(order);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void strategy_base_t::move_order(const order_base_t& order, double price)
{
    m_robot.move_order(order, price);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


