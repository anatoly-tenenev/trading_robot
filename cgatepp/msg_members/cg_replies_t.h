#ifndef CG_REPLIES_T_H
#define CG_REPLIES_T_H

#include "../cg_headers.h"

#include "../cg_member_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_replies_t : public cg_member_base_t
{
public:
    cg_replies_t(cg_message_base_t* message, const vector<u_int64_t>& replies) :
        cg_member_base_t(message, "replies"),
        m_replies(replies)
    {

    }

    string to_ini() const override
    {
        stringstream result;
        result << m_name << "=";
        for (size_t i = 0; i < m_replies.size(); i++)
        {
            if (i != 0)
                result << ",";
            result << m_replies[i];
        }
        return result.str();
    }

private:
    vector<u_int64_t> m_replies;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


#endif // CG_REPLIES_T_H
