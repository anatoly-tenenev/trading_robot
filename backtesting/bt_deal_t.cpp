#include "bt_deal_t.h"
#include "bt_symbol_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bt_deal_t::bt_deal_t(const bt_symbol_t& symbol,
                     deal_type_t type,
                     double price,
                     vol_t volume,
                     int order_id,
                     const ptime& datetime) :
    deal_base_t{symbol, type, price, volume, order_id, datetime}
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bt_deal_t::bt_deal_t(const bt_symbol_t& symbol,
                     deal_type_t type,
                     double price,
                     vol_t volume,
                     int order_id) :
    bt_deal_t{symbol, type, price, volume, order_id, ptime{min_date_time}}
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bt_deal_t::bt_deal_t(const bt_symbol_t& symbol,
                     deal_type_t type,
                     double price,
                     vol_t volume) :
    bt_deal_t{symbol, type, price, volume, 0, ptime{min_date_time}}
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bt_deal_t::bt_deal_t(const bt_symbol_t& symbol,
                     deal_type_t type,
                     double price,
                     vol_t volume,
                     const ptime& datetime) :
    bt_deal_t{symbol, type, price, volume, 0, datetime}
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
