#ifndef CG_MEMBER_BASE_T_H
#define CG_MEMBER_BASE_T_H

#include "cg_headers.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_member_base_t
{
public:
    cg_member_base_t(cg_message_base_t* message, const string& name);
    cg_member_base_t(const cg_member_base_t& that) = delete;
    cg_member_base_t& operator=(const cg_member_base_t& that) = delete;
    virtual ~cg_member_base_t();

    virtual string to_ini() const = 0;

protected:
    string m_name;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_MEMBER_BASE_T_H
