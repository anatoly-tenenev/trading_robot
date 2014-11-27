#ifndef BT_DEAL_T_H
#define BT_DEAL_T_H

#include "bt_headers.h"
#include "../deal_base_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class bt_deal_t : public trading::deal_base_t
{
public:
    bt_deal_t(const bt_symbol_t& symbol,
              deal_type_t type,
              double price,
              vol_t volume,
              int order_id,
              const ptime& datetime);

    bt_deal_t(const bt_symbol_t& symbol,
              deal_type_t type,
              double price,
              vol_t volume,
              int order_id);

    bt_deal_t(const bt_symbol_t& symbol,
              deal_type_t type,
              double price,
              vol_t volume);

    bt_deal_t(const bt_symbol_t& symbol,
              deal_type_t type,
              double price,
              vol_t volume,
              const ptime& datetime);

    bool operator== (const bt_deal_t& deal) const
    {
        if (this == &deal) return true;
        if (!deal_base_t::equal(deal))
            return false;
        return true;
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_DEAL_T_H
