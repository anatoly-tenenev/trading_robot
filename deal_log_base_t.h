#ifndef DEAL_LOG_BASE_T_H
#define DEAL_LOG_BASE_T_H

#include "headers.h"
#include "types.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class deal_log_base_t
{
protected:
    map<double, vol_t> m_deals;
    deal_type_t m_state = deal_type_t::undefined;

public:
    deal_log_base_t();
    virtual ~deal_log_base_t();

    vol_t operator[] (double price) const;
    void set(double price, vol_t volume);

    void state(deal_type_t state) { m_state = state; }
    deal_type_t state() const { return m_state; }

    vector<double> sells() const;
    vector<double> buys() const;

    void add_deal(const deal_base_t& deal);

protected:

    virtual bool equal(const deal_log_base_t& deal_log) const
    {
        if (this == &deal_log)
            return true;
        if (m_state != deal_log.m_state)
            return false;
        if (m_deals != deal_log.m_deals)
            return false;
        return true;
    }

    virtual void copy(const deal_log_base_t& deal_log)
    {
        if (this == &deal_log)
            return;
        m_state = deal_log.m_state;
        m_deals = deal_log.m_deals;
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // DEAL_LOG_BASE_T_H
