#ifndef BT_ORDERLOG_T_H
#define BT_ORDERLOG_T_H

#include "bt_headers.h"
#include "../orderlog_t.hpp"
#include "bt_order_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class bt_orderlog_t : public trading::orderlog_t<bt_order_t>
{
    friend class bt_market_t;

public:
    bt_orderlog_t() {}

    bool operator== (const bt_orderlog_t& orderlog) const
    {
        if (this == &orderlog)
            return true;
        if (!orderlog_t<bt_order_t>::equal(orderlog))
            return false;
        return true;
    }

    virtual bool is_bids_sorted() override;
    virtual bool is_asks_sorted() override;
    virtual void sort_bids() override;
    virtual void sort_asks() override;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_ORDERLOG_T_H
