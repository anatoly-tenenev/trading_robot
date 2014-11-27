#ifndef SCALPER_ORDER_T_H
#define SCALPER_ORDER_T_H

#include "scalper_headers.h"
#include "../headers.h"
#include "../order_base_t.h"

namespace strategies
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class scalper_order_t : public order_base_t
{
public:
    scalper_order_t(const symbol_base_t& symbol,
            order_dir_t dir, double price, vol_t volume) :
        order_base_t{symbol, dir, price, volume, ptime{min_date_time}}
    {
    }

    scalper_order_t& operator= (const scalper_order_t& order)
    {
        if (this == &order)
            return *this;
        order_base_t::copy(order);
        m_pending = order.m_pending;
        m_start_pending_time = order.m_start_pending_time;
        m_for_cancel = order.m_for_cancel;
        m_orders_before = order.m_orders_before;
        return *this;
    }

    bool is_pending() const { return m_pending; }
    void start_pending(const ptime& start_pending_time)
    {
        m_pending = true;
        m_start_pending_time = start_pending_time;
    }
    void stop_pending()
    {
        m_pending = false;
        m_start_pending_time = ptime{min_date_time};
    }
    ptime start_pending_time() const { return m_start_pending_time; }

    bool is_for_cancel() const { return m_for_cancel; }
    void for_cancel(bool for_cancel) { m_for_cancel = for_cancel; }

    void orders_before(int orders_before) override { m_orders_before = orders_before; }
    int orders_before() const override { return m_orders_before; }

    void is_init(bool is_init) { m_is_init = is_init; }
    bool is_init() const { return m_is_init; }

    void target_price(double target_price) { m_target_price = target_price; }
    double target_price() const { return m_target_price; }

protected:
    double m_target_price = 0.0;
    bool m_pending = true;
    ptime m_start_pending_time = ptime{min_date_time};
    bool m_for_cancel = false;
    int m_orders_before = 0;
    bool m_is_init = false;

    void abstract_func() override {}
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // SCALPER_ORDER_T_H
