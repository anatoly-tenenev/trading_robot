#ifndef BT_ORDER_T_H
#define BT_ORDER_T_H

#include "bt_headers.h"
#include "../order_t.h"
#include "bt_symbol_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class bt_order_t : public trading::order_t
{
public:
    bt_order_t(const bt_symbol_t& symbol,
               order_dir_t dir, double price, vol_t volume,
               const ptime& datetime) :
        order_t{symbol, dir, price, volume, datetime}
    {
    }

    bt_order_t(const bt_symbol_t& symbol,
               order_dir_t dir, double price, vol_t volume) :
        bt_order_t{symbol, dir, price, volume, ptime{min_date_time}}
    {
    }

//    bt_order_t(const bt_symbol_t& symbol,
//               order_dir_t dir, double price, vol_t volume,
//               int orders_before) :
//        bt_order_t{symbol, dir, price, volume, ptime{min_date_time}},
//        m_orders_before{orders_before}
//    {
//    }

    using trading::order_t::id;
    void id(int id) { m_id = id; }

    bt_order_t& operator= (const bt_order_t& order)
    {
        if (this == &order)
            return *this;
        order_t::copy(order);
        m_orders_before = order.m_orders_before;
        return *this;
    }

    bool operator== (const bt_order_t& order) const
    {
        if (this == &order)
            return true;
        if (!order_t::equal(order))
            return false;
        if (m_orders_before != order.m_orders_before)
            return false;
        return true;
    }

    void orders_before(int orders_before) override { m_orders_before = orders_before; }
    int orders_before() const override { return m_orders_before; }

    static bool bids_compare(const bt_order_t& bid1, const bt_order_t& bid2);
    static bool asks_compare(const bt_order_t& ask1, const bt_order_t& ask2);

protected:
    int m_orders_before = 0;

    void abstract_func() override {}
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_ORDER_T_H
