#include "order_base_t.h"
#include "symbol_base_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int order_base_t::s_counter = 0;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

order_base_t::order_base_t(const symbol_base_t& symbol,
                           order_dir_t dir,
                           double price,
                           vol_t volume,
                           const ptime& datetime) :
    m_id{++s_counter},
    m_symbol(symbol),
    m_dir{dir},
    m_price{price},
    m_volume{volume},
    m_datetime{datetime}
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

order_base_t::order_base_t(const symbol_base_t &symbol) :
    order_base_t{symbol, order_dir_t::bid, 0.0, 0, ptime{min_date_time}}
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

order_base_t::~order_base_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bool order_base_t::bids_compare(const order_base_t& bid1, const order_base_t& bid2)
{
    if (bid1.price() > bid2.price())
        return true;
    else if (bid1.price() == bid2.price())
    {
        if (bid1.datetime() < bid2.datetime())
            return true;
        else
            return false;
    }
    else
        return false;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bool order_base_t::asks_compare(const order_base_t& ask1, const order_base_t& ask2)
{
    if (ask1.price() < ask2.price())
        return true;
    else if (ask1.price() == ask2.price())
    {
        if (ask1.datetime() < ask2.datetime())
            return true;
        else
            return false;
    }
    else
        return false;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
