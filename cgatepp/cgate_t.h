#ifndef CGATE_T_H
#define CGATE_T_H

#include "cg_headers.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cgate_t
{
public:
    cgate_t(const string& settings);
    cgate_t(const cgate_t& that) = delete;
    cgate_t& operator=(const cgate_t& that) = delete;
    ~cgate_t();

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CGATE_T_H
