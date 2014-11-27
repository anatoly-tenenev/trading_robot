#ifndef MARKET_EVENT_T_H
#define MARKET_EVENT_T_H

#include "headers.h"
#include "symbol_base_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class market_event_t
{
public:
    const event_type_t type;
    ptime datetime;
    const symbol_base_t& symbol;

    market_event_t(event_type_t _type, const symbol_base_t& _symbol) :
        type{_type},
        symbol(_symbol)
    {
    }

    virtual ~market_event_t() {}

protected:

    virtual void copy(const market_event_t& market_event)
    {
        if (this == &market_event)
            return;
        datetime = market_event.datetime;
    }

    virtual void abstract_func() = 0;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_MARKET_EVENT_T_H
