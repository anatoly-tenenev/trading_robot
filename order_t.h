#ifndef ORDER_T_H
#define ORDER_T_H

#include "headers.h"
#include "order_base_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class order_t : public order_base_t
{
public:
    order_t(const symbol_base_t& symbol,
            order_dir_t dir, double price, vol_t volume,
            const ptime& datetime) :
        order_base_t{symbol, dir, price, volume, datetime}
    {
    }
    order_t(const symbol_base_t& symbol) :
        order_t{symbol, order_dir_t::bid, 0.0, 0, ptime{min_date_time}}
    {
    }
    virtual ~order_t() {}

protected:

    bool equal(const order_t& order) const
    {
        if (this == &order)
            return true;
        if (!order_base_t::equal(order))
            return false;
        return true;
    }

    void copy(const order_t& order)
    {
        if (this == &order)
            return;
        order_base_t::copy(order);
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // ORDER_T_H
