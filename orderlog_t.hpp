#ifndef ORDERLOG_T_H
#define ORDERLOG_T_H

#include "headers.h"
#include "orderlog_base_t.h"
#include "order_base_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template <typename OrderT>
class orderlog_t : public orderlog_base_t
{
public:
    orderlog_t();
    virtual ~orderlog_t();

    order_array_t bids() override;
    order_array_t asks() override;

    void add(const OrderT& order);

    void clean_up() override;
    static void clean_up(vector<OrderT>& orders);

protected:
    vector<OrderT> m_bids;
    vector<OrderT> m_asks;

    virtual bool is_bids_sorted();
    virtual bool is_asks_sorted();
    virtual void sort_bids();
    virtual void sort_asks();

    virtual bool equal(const orderlog_t<OrderT>& orderlog) const
    {
        if (this == &orderlog)
            return true;
        if (m_bids != orderlog.m_bids)
            return false;
        if (m_asks != orderlog.m_asks)
            return false;
        return true;
    }

    virtual void copy(const orderlog_t<OrderT>& orderlog)
    {
        if (this == &orderlog)
            return;
        m_bids = orderlog.m_bids;
        m_asks = orderlog.m_asks;
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

template <typename OrderT>
orderlog_t<OrderT>::orderlog_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template <typename OrderT>
orderlog_t<OrderT>::~orderlog_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template <typename OrderT>
order_array_t orderlog_t<OrderT>::bids()
{
    clean_up(m_bids);
    if ( !is_bids_sorted() )
        sort_bids();
    return order_array_t{m_bids};
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template <typename OrderT>
order_array_t orderlog_t<OrderT>::asks()
{
    clean_up(m_asks);
    if ( !is_asks_sorted() )
        sort_asks();
    return order_array_t{m_asks};
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template <typename OrderT>
void orderlog_t<OrderT>::add(const OrderT& order)
{
    switch( order.dir() )
    {

    case order_dir_t::bid:
    {
        auto it = std::find_if(m_bids.begin(), m_bids.end(),
                               [&order](const OrderT& _order) -> bool {
                                   return order_base_t::bids_compare(order, _order);
                               });
        m_bids.insert(it, order);
        if ( is_bids_sorted() )
            break;
        else
            sort_bids();

    } break;

    case order_dir_t::ask:
    {
        auto it = std::find_if(m_asks.begin(), m_asks.end(),
                               [&order](const OrderT& _order) -> bool {
                                   return order_base_t::asks_compare(order, _order);
                               });
        m_asks.insert(it, order);
        if ( is_asks_sorted() )
            break;
        else
            sort_asks();

    } break;

    default:
        break;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template <typename OrderT>
bool orderlog_t<OrderT>::is_bids_sorted()
{
    return std::is_sorted(m_bids.begin(), m_bids.end(), order_base_t::bids_compare);
}

template <typename OrderT>
void orderlog_t<OrderT>::sort_bids()
{
    std::sort(m_bids.begin(), m_bids.end(), order_base_t::bids_compare);
}

template <typename OrderT>
bool orderlog_t<OrderT>::is_asks_sorted()
{
    return std::is_sorted(m_asks.begin(), m_asks.end(), order_base_t::asks_compare);
}

template <typename OrderT>
void orderlog_t<OrderT>::sort_asks()
{
    std::sort(m_asks.begin(), m_asks.end(), order_base_t::asks_compare);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template <typename OrderT>
void orderlog_t<OrderT>::clean_up()
{
    clean_up(m_bids);
    clean_up(m_asks);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template <typename OrderT>
void orderlog_t<OrderT>::clean_up(vector<OrderT>& orders)
{
    orders.erase(std::remove_if(orders.begin(), orders.end(),
        [](const OrderT& order) -> bool
        {
            return order.volume() <= 0;
        }
    ), orders.end());
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // ORDERLOG_T_H
