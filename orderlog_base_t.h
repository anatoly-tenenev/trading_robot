#ifndef ORDERLOG_BASE_T_H
#define ORDERLOG_BASE_T_H

#include "headers.h"
#include "order_array_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class orderlog_base_t
{
public:
    orderlog_base_t();
    virtual ~orderlog_base_t();

    virtual order_array_t bids() = 0;
    virtual order_array_t asks() = 0;

    virtual void clean_up() = 0;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // ORDERLOG_BASE_T_H
