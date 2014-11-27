#ifndef BT_ORDERBOOK_T_H
#define BT_ORDERBOOK_T_H

#include "bt_headers.h"
#include "../orderbook_base_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class bt_orderbook_t : public trading::orderbook_base_t
{
public:
    bt_orderbook_t()
    {

    }

    bt_orderbook_t(const bt_orderbook_t& orderbook) :
        orderbook_base_t(orderbook)
    {

    }

    bt_orderbook_t& operator= (const bt_orderbook_t& orderbook)
    {
        if (this == &orderbook)
            return *this;
        orderbook_base_t::copy(orderbook);
        return *this;
    }

    bool operator== (const bt_orderbook_t& orderbook) const
    {
        if (this == &orderbook)
            return true;
        if (!orderbook_base_t::equal(orderbook))
            return false;
        return true;
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_ORDERBOOK_T_H
