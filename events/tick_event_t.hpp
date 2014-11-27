#ifndef TICK_EVENT_T_HPP
#define TICK_EVENT_T_HPP

#include "../headers.h"
#include "../market_event_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class tick_event_t : public market_event_t
{
public:
    double price;
    vol_t volume;
    deal_type_t tick_type;

    tick_event_t(const symbol_base_t& _symbol) :
        market_event_t{event_type_t::tick, _symbol}
    {

    }

    tick_event_t& operator= (const tick_event_t& tick_event)
    {
        if (this == &tick_event)
            return *this;
        market_event_t::copy(tick_event);
        price = tick_event.price;
        volume = tick_event.volume;
        tick_type = tick_event.tick_type;
        return *this;
    }

protected:
    virtual void abstract_func() override {}
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_TICK_EVENT_T_HPP
