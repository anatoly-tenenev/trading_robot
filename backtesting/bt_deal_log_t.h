#ifndef BT_DEAL_LOG_T_H
#define BT_DEAL_LOG_T_H

#include "bt_headers.h"
#include "../deal_log_base_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class bt_deal_log_t : public trading::deal_log_base_t
{
public:
    bt_deal_log_t();

    bool operator== (const bt_deal_log_t& deal_log) const
    {
        if (this == &deal_log)
            return true;
        if (!deal_log_base_t::equal(deal_log))
            return false;
        return true;
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_DEAL_LOG_T_H
