#ifndef ORDERBOOK_EVENT_T_HPP
#define ORDERBOOK_EVENT_T_HPP

#include "../headers.h"
#include "../market_event_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class orderbook_event_t : public market_event_t
{
public:
    map< double, vol_t, std::less<vol_t> >    asks;
    map< double, vol_t, std::greater<vol_t> > bids;

    orderbook_event_t(const symbol_base_t& _symbol) :
        market_event_t{event_type_t::orderbook, _symbol}
    {

    }

    orderbook_event_t& operator= (const orderbook_event_t& orderbook_event)
    {
        if (this == &orderbook_event)
            return *this;
        market_event_t::copy(orderbook_event);
        asks = orderbook_event.asks;
        bids = orderbook_event.bids;
        return *this;
    }

protected:
    virtual void abstract_func() override {}
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // ORDERBOOK_EVENT_T_HPP
