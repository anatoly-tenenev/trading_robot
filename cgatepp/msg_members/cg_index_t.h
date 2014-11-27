#ifndef CG_INDEX_T_H
#define CG_INDEX_T_H

#include "../cg_headers.h"

#include "../cg_member_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_index_t : public cg_member_base_t
{
public:
    cg_index_t(cg_message_base_t* message, const string& text) :
        cg_member_base_t(message, "index"),
        m_text(text)
    {

    }

    string to_ini() const override
    {
        return m_name + string("=") + m_text;
    }

private:
    string m_text;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_INDEX_T_H
