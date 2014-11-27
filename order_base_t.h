#ifndef ORDER_BASE_T_H
#define ORDER_BASE_T_H

#include "headers.h"
#include "symbol_base_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class order_base_t
{
public:
    order_base_t(const symbol_base_t& symbol,
                 order_dir_t dir,
                 double price,
                 vol_t volume,
                 const ptime& datetime);
    order_base_t(const symbol_base_t& symbol);
    virtual ~order_base_t();

    int id() const { return m_id; }

    ptime datetime() const { return m_datetime; }
    void datetime(const ptime& datetime) { m_datetime = datetime; }

    const symbol_base_t& symbol() const { return m_symbol; }

    order_type_t type() const { return m_type; }
    void type(order_type_t type) { m_type = type; }

    order_dir_t dir() const { return m_dir; }
    void dir(order_dir_t dir) { m_dir = dir; }

    double price() const { return m_price; }
    void price(double price) { m_price = price; }

    vol_t volume() const { return m_volume; }
    void volume(vol_t volume) { m_volume = volume; }

    virtual void orders_before(int orders_before) = 0;
    virtual int orders_before() const = 0;

    static bool bids_compare(const order_base_t& bid1, const order_base_t& bid2);
    static bool asks_compare(const order_base_t& ask1, const order_base_t& ask2);

protected:
    int m_id;
    const symbol_base_t& m_symbol;
    order_dir_t m_dir;
    double m_price;
    vol_t m_volume;
    order_type_t m_type = order_type_t::limit;
    ptime m_datetime;

    static int s_counter;

    virtual bool equal(const order_base_t& order) const
    {
        if (this == &order)
            return true;
        if (m_id != order.m_id)
            return false;
        if (!m_symbol.equal(order.m_symbol))
            return false;
        if (m_dir != order.m_dir)
            return false;
        if (m_price != order.m_price)
            return false;
        if (m_volume != order.m_volume)
            return false;
        if (m_type != order.m_type)
            return false;
        if (m_datetime != order.m_datetime)
            return false;
        return true;
    }

    virtual void copy(const order_base_t& order)
    {
        if (this == &order)
            return;
        assert( m_symbol.equal(order.symbol()) );
        m_id = order.m_id;
        m_dir = order.m_dir;
        m_price = order.m_price;
        m_volume = order.m_volume;
        m_type = order.m_type;
        m_datetime = order.m_datetime;
    }

    virtual void abstract_func() = 0;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // ORDER_BASE_T_H
