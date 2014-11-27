#ifndef CG_UTILS_H
#define CG_UTILS_H

#include "cg_headers.h"
#include "../utils.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

static const double powers_of_10_array[] = { 1.0, 10.0, 100.0, 1000.0, 10000.0, 100000.0, 1000000.0, 10000000.0 };

inline
double
powers_of_10(int8_t scale)
{
    if ((scale >= 0) && (scale <= 7))
        return powers_of_10_array[scale];
    else
        return std::pow(10.0, scale);
}

inline
double
bcd_to_double(const char* bcd)
{
    int64_t intpart;
    int8_t scale;
    ::cg_bcd_get(static_cast<const void*>(bcd), &intpart, &scale); // cgate 1.3.8
    //::cg_bcd_get(static_cast<void*>(const_cast<char*>(bcd)), &intpart, &scale); // cgate 1.3.4
    return static_cast<double>(intpart) / powers_of_10(scale);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

inline size_t bcd_len(size_t N, size_t M)
{
    return 2 + (N >> 1) + ((N | M) & 1);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bool msg_by_index_compare(cg_message_base_t* msg1, cg_message_base_t* msg2);

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_UTILS_H
