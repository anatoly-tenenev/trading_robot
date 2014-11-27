#ifndef ORDER_ARRAY_T_H
#define ORDER_ARRAY_T_H

#include "headers.h"
#include "utils.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class order_array_t
{
    const char* m_data;
    const size_t m_step;
    const size_t m_size;

public:
    template<typename OrderT>
    order_array_t(const vector<OrderT>& orders) :
        m_data{ pointer_cast<char*>(orders.data()) },
        m_step{ sizeof(OrderT) },
        m_size{ orders.size() }
    {
    }

    size_t size() const { return m_size; }
    order_t& operator[] (size_t i) const
    {
        return *pointer_cast<order_t*>(m_data + i*m_step);
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // ORDER_ARRAY_T_H
