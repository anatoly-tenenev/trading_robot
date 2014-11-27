#ifndef DEAL_BASE_T_H
#define DEAL_BASE_T_H

#include "headers.h"
#include "symbol_base_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class deal_base_t
{
public:
    deal_base_t(const symbol_base_t& symbol,
                deal_type_t type,
                double price,
                vol_t volume,
                int order_id,
                const ptime& datetime);
    virtual ~deal_base_t();

    deal_base_t& operator= (const deal_base_t& deal)
    {
        if (this == &deal)
            return *this;
        copy(deal);
        return *this;
    }

    double price() const { return m_price; }
    void price(double price) { m_price = price; }

    vol_t volume() const { return m_volume; }
    void volume(vol_t volume) { m_volume = volume; }

    const symbol_base_t& symbol() const { return m_symbol; }
    deal_type_t type() const { return m_type; }
    int order_id() const { return m_order_id; }

    void datetime(const ptime& datetime) { m_datetime = datetime; }
    ptime datetime() const { return m_datetime; }

protected:
    const symbol_base_t& m_symbol;
    deal_type_t m_type;
    double m_price;
    vol_t m_volume;
    int m_order_id;
    ptime m_datetime;

    virtual bool equal(const deal_base_t& deal) const
    {
        if (this == &deal)
            return true;
        if (!m_symbol.equal(deal.m_symbol))
            return false;
        if (m_type != deal.m_type)
            return false;
        if (m_price != deal.m_price)
            return false;
        if (m_volume != deal.m_volume)
            return false;
        if (m_order_id != deal.m_order_id)
            return false;
        if (m_datetime != deal.m_datetime)
            return false;
        return true;
    }

    virtual void copy(const deal_base_t& deal)
    {
        if (this == &deal)
            return;
        assert(m_symbol.equal(deal.m_symbol));
        m_type = deal.m_type;
        m_price = deal.m_price;
        m_volume = deal.m_volume;
        m_order_id = deal.m_order_id;
        m_datetime = deal.m_datetime;
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // DEAL_BASE_T_H
