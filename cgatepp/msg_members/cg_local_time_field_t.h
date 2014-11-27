#ifndef CG_LOCAL_TIME_FIELD_T_H
#define CG_LOCAL_TIME_FIELD_T_H

#include "../cg_headers.h"

#include "../cg_member_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_local_time_field_t : public cg_member_base_t
{
public:
    cg_local_time_field_t(cg_message_base_t* message, const string& value) :
        cg_member_base_t(message, "LocalTimeField"),
        m_value(value)
    {

    }

    string to_ini() const override
    {
        return m_name + string("=") + m_value;
    }

private:
    string m_value;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_LOCAL_TIME_FIELD_T_H
