#ifndef CG_REPLY_T_H
#define CG_REPLY_T_H

#include "../cg_headers.h"

#include "../cg_member_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_reply_t : public cg_member_base_t
{
public:
    cg_reply_t(cg_message_base_t* message) :
        cg_member_base_t(message, "reply")
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


#endif // CG_REPLY_T_H
