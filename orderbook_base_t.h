#ifndef ORDERBOOK_BASE_T_H
#define ORDERBOOK_BASE_T_H

#include "headers.h"
#include "types.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class orderbook_base_t
{
protected:
    map< double, vol_t, std::less<vol_t> >    m_asks;
    map< double, vol_t, std::greater<vol_t> > m_bids;

public:
    orderbook_base_t();
    orderbook_base_t(const orderbook_base_t& orderbook);
    virtual ~orderbook_base_t();

    double best_bid() const { return m_bids.begin()->first; }
    double best_ask() const { return m_asks.begin()->first; }

    vol_t operator[] (double price) const;

    quote_accessor_t<decltype(m_bids)> bids{m_bids};
    quote_accessor_t<decltype(m_asks)> asks{m_asks};

    void set(order_dir_t dir, double price, vol_t volume);

    void set_bids(const decltype(m_bids)& bids) { m_bids = bids; }
    void set_asks(const decltype(m_asks)& asks) { m_asks = asks; }

    decltype(m_bids) get_bids() const { return m_bids; }
    decltype(m_asks) get_asks() const { return m_asks; }

    vector<double> bids_prices() const
    {
        vector<double> prices;
        for (auto it = m_bids.begin(); it != m_bids.end(); ++it)
            prices.push_back( it->first );
        return prices;
    }

    vector<double> asks_prices() const
    {
        vector<double> prices;
        for (auto it = m_asks.begin(); it != m_asks.end(); ++it)
            prices.push_back( it->first );
        return prices;
    }

    int orders_before(const order_base_t& order) const;

protected:

    virtual bool equal(const orderbook_base_t& orderbook) const
    {
        if (this == &orderbook)
            return true;
        if (m_bids != orderbook.m_bids)
            return false;
        if (m_asks != orderbook.m_asks)
            return false;
        return true;
    }

    virtual void copy(const orderbook_base_t& orderbook)
    {
        if (this == &orderbook)
            return;
        m_bids = orderbook.m_bids;
        m_asks = orderbook.m_asks;
    }

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // ORDERBOOK_BASE_T_H
