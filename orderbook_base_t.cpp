#include "orderbook_base_t.h"
#include "order_base_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

orderbook_base_t::orderbook_base_t()
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

orderbook_base_t::orderbook_base_t(const orderbook_base_t& orderbook) :
    bids{m_bids},
    asks{m_asks}
{
    m_bids = orderbook.m_bids;
    m_asks = orderbook.m_asks;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

orderbook_base_t::~orderbook_base_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

vol_t orderbook_base_t::operator[] (double price) const
{
    if (price >= best_ask())
    {
        auto it = m_asks.find(price);
        if (it != m_asks.end())
            return it->second;
        else
            return 0;
    }

    if (price <= best_bid())
    {
        auto it = m_bids.find(price);
        if (it != m_bids.end())
            return it->second;
        else
            return 0;
    }

    return 0;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void orderbook_base_t::set(order_dir_t dir, double price, vol_t volume)
{
    switch (dir)
    {
    case order_dir_t::bid:
        bids.set(price, volume);
        break;
    case order_dir_t::ask:
        asks.set(price, volume);
        break;
    default:
        break;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int orderbook_base_t::orders_before(const order_base_t& order) const
{
    if (order.dir() == order_dir_t::bid)
    {
        if (order.price() >= best_ask())
            return 0;
        else
            return (*this)[order.price()];
    }
    else
    {
        if (order.price() <= best_bid())
            return 0;
        else
            return (*this)[order.price()];
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


