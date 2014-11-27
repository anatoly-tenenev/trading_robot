#include "bt_robot_t.h"
#include "bt_market_t.h"
#include "bt_task_t.h"
#include "bt_order_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bt_robot_t::bt_robot_t(bt_market_t& market) :
    m_market(market)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_robot_t::add_order(const trading::order_base_t& order)
{
    const bt_order_t& bt_order = static_cast<const bt_order_t&>(order);

    m_market.add_task(
        m_market.task_time(),
        [&market = m_market, bt_order]() {
            market.add_order(bt_order);
    });
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_robot_t::del_order(const trading::order_base_t& order)
{
    const bt_order_t& bt_order = static_cast<const bt_order_t&>(order);

    m_market.add_task(
        m_market.task_time(),
        [&market = m_market, bt_order]() {
            market.del_order(bt_order);
    });
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void bt_robot_t::move_order(const trading::order_base_t& order, double price)
{
    const bt_order_t& bt_order = static_cast<const bt_order_t&>(order);

    m_market.add_task(
        m_market.task_time(),
        [&market = m_market, bt_order, price]() {
            market.move_order(bt_order, price);
    });
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

