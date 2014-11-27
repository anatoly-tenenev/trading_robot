#ifndef CG_MSGID_T_H
#define CG_MSGID_T_H

#include "../cg_headers.h"

#include "../cg_member_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_msgid_t : public cg_member_base_t
{
public:
    cg_msgid_t(cg_message_base_t* message, const u_int64_t value) :
        cg_member_base_t(message, "msgid"),
        m_value(value)
    {

    }

    string to_ini() const override
    {
        return m_name + string("=") + std::to_string(m_value);
    }

private:
    u_int64_t m_value;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


#endif // CG_MSGID_T_H
