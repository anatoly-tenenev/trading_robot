#include "bt_order_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bool bt_order_t::bids_compare(const bt_order_t &bid1, const bt_order_t &bid2)
{
    if (bid1.price() > bid2.price())
        return true;
    else if (bid1.price() == bid2.price())
    {
        if (bid1.datetime() < bid2.datetime())
            return true;
        else if (bid1.datetime() == bid2.datetime())
        {
            if (bid1.orders_before() < bid2.orders_before())
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bool bt_order_t::asks_compare(const bt_order_t &ask1, const bt_order_t &ask2)
{
    if (ask1.price() < ask2.price())
        return true;
    else if (ask1.price() == ask2.price())
    {
        if (ask1.datetime() < ask2.datetime())
            return true;
        else if (ask1.datetime() == ask2.datetime())
        {
            if (ask1.orders_before() < ask2.orders_before())
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
