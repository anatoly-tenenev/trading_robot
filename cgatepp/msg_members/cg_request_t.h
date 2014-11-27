#ifndef CG_REQUEST_T_H
#define CG_REQUEST_T_H

#include "../cg_headers.h"

#include "../cg_member_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_request_t : public cg_member_base_t
{
public:
    cg_request_t(cg_message_base_t* message) :
        cg_member_base_t(message, "request")
    {

    }

    string to_ini() const override
    {
        return m_name + string("=1");
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_REQUEST_T_H
