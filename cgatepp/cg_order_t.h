#ifndef CG_ORDER_T_H
#define CG_ORDER_T_H

#include "cg_headers.h"
#include "../order_t.h"
#include "cg_symbol_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_order_t : public trading::order_t
{
public:
    cg_order_t(const cg_symbol_t& symbol) :
        order_t{symbol}
    {

    }

    bool operator== (const cg_order_t& order) const
    {
        if (this == &order)
            return true;
        if (!order_base_t::equal(order))
            return false;
        if (m_orders_before != order.m_orders_before)
            return false;
        return true;
    }

    cg_order_t& operator= (const cg_order_t& order)
    {
        if (this == &order)
            return *this;
        order_t::copy(order);
        m_orders_before = order.m_orders_before;
        return *this;
    }

    void orders_before(int orders_before) override { m_orders_before = orders_before; }
    int orders_before() const override { return m_orders_before; }

protected:
    int m_orders_before = 0;

    void abstract_func() override {}
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_ORDER_T_H
