#ifndef SESSION_MSG_T_H
#define SESSION_MSG_T_H

#include "../cg_message_t.h"
#include "../cg_field_t.h"
#include "../msg_members/cg_index_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class session_msg_t : public cg_message_t
{
public:
    session_msg_t(cg_stream_t* stream) : cg_message_t(stream, "session")
    {
        check_fields();
    }
    ~session_msg_t() {}

    struct session_t
    {
        int64_t replID;
        int64_t replRev;
        int64_t replAct;
    };

    map<int64_t, session_t> table;
    session_t msg;

    cg_field_t<decltype(session_t::replID)>  f_replID{this, "replID", "i8", msg.replID};
    cg_field_t<decltype(session_t::replRev)> f_replRev{this, "replRev", "i8", msg.replRev};
    cg_field_t<decltype(session_t::replAct)> f_replAct{this, "replAct", "i8", msg.replAct};

    cg_index_t index1{this, "IDX_session_ID,UNIQUE,replID"};
    cg_index_t index2{this, "IDX_session_REV_U,UNIQUE,replRev"};

    void update_table() override
    {
        if (msg.replAct != 0)
            table.erase(msg.replID);
        else
            table[msg.replID] = msg;
    }

    void cleardeleted(int64_t replRev) override
    {
        cleardeleted_table(table, replRev);
    }

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // SESSION_MSG_T_H
