#include "deal_base_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

deal_base_t::deal_base_t(const symbol_base_t& symbol,
                         deal_type_t type,
                         double price,
                         vol_t volume,
                         int order_id,
                         const ptime& datetime) :
    m_symbol(symbol),
    m_type{type},
    m_price{price},
    m_volume{volume},
    m_order_id{order_id},
    m_datetime{datetime}

{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

deal_base_t::~deal_base_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
