#ifndef CG_ORDERLOG_T_H
#define CG_ORDERLOG_T_H

#include "cg_headers.h"
#include "../orderlog_t.hpp"
#include "cg_order_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_orderlog_t : public trading::orderlog_t<cg_order_t>
{
public:
    cg_orderlog_t();

private:

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_ORDERLOG_T_H
