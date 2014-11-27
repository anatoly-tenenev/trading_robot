#ifndef ORDBOOK_INFO_MSG_T_H
#define ORDBOOK_INFO_MSG_T_H

#include "../cg_message_t.h"
#include "../cg_field_t.h"
#include "../msg_members/cg_index_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class ordbook_info_msg_t : public cg_message_t
{
public:
    ordbook_info_msg_t(cg_stream_t* stream) : cg_message_t(stream, "info")
    {
        check_fields();
    }

    struct info_t
    {
        int64_t replID;
        int64_t replRev;
        int64_t replAct;
        int64_t infoID;
        int64_t logRev;
        ptime moment;
    };

    info_t msg;

    cg_field_t<decltype(info_t::replID)>  f_replID{this, "replID", "i8", msg.replID};
    cg_field_t<decltype(info_t::replRev)> f_replRev{this, "replRev", "i8", msg.replRev};
    cg_field_t<decltype(info_t::replAct)> f_replAct{this, "replAct", "i8", msg.replAct};
    cg_field_t<decltype(info_t::infoID)> f_infoID{this, "infoID", "i8", msg.infoID};
    cg_field_t<decltype(info_t::logRev)> f_logRev{this, "logRev", "i8", msg.logRev};
    cg_field_t<decltype(info_t::moment)> f_moment{this, "moment", "t", msg.moment};

    cg_index_t index1{this, "IDX_info_ID,UNIQUE,replID"};
    cg_index_t index2{this, "IDX_info_REV_U,UNIQUE,replRev"};

    void update_table() override
    {
    }

    void cleardeleted(int64_t replRev) override
    {
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // ORDBOOK_INFO_MSG_T_H
