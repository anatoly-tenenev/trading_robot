#ifndef TYPES_H
#define TYPES_H

#include "headers.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

struct quote_t
{
    double price;
    int volume;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<typename T>
class quote_accessor_t
{
public:
    quote_accessor_t(T& quotes) : m_quotes(quotes) {}

    vol_t operator[] (double price) const
    {
        auto it = m_quotes.find(price);
        if (it != m_quotes.end())
            return it->second;
        else
            return 0;
    }

    void set(double price, vol_t volume)
    {
        if (volume <= 0)
            m_quotes.erase(price);
        else
            m_quotes[price] = volume;
    }

private:
    T& m_quotes;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


#endif // TYPES_H
