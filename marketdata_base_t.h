#ifndef MARKETDATA_BASE_T_H
#define MARKETDATA_BASE_T_H

#include "headers.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class marketdata_base_t
{
public:
    marketdata_base_t();
    virtual ~marketdata_base_t();

    virtual orderlog_base_t& orderlog() = 0;
    virtual orderbook_base_t& orderbook() = 0;
    virtual deal_log_base_t& deal_log() = 0;

protected:

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // MARKETDATA_BASE_T_H
