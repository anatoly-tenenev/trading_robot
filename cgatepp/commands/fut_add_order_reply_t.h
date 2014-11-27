#ifndef FUT_ADD_ORDER_REPLY_T_H
#define FUT_ADD_ORDER_REPLY_T_H

#include "../cg_message_mqreply_t.h"
#include "../cg_field_t.h"
#include "../msg_members/cg_msgid_t.h"
#include "../msg_members/cg_reply_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class fut_add_order_reply_t : public cg_message_mqreply_t
{
public:
    fut_add_order_reply_t(cg_stream_mqreply_t* stream) :
        cg_message_mqreply_t(stream, "FORTS_MSG101")
    {
        check_fields();
    }
    ~fut_add_order_reply_t() {}

    struct msg_t
    {
        int32_t code;
        string message;
        int64_t order_id;
    };

    msg_t msg;

    cg_field_t<decltype(msg_t::code)>     code{this, "code", "i4", msg.code};
    cg_field_t<decltype(msg_t::message)>  message{this, "message", "c255", msg.message};
    cg_field_t<decltype(msg_t::order_id)> order_id{this, "order_id", "i8", msg.order_id};

    cg_msgid_t   msgid{this, 101};
    cg_reply_t   reply{this};

    void event() override
    {

    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // FUT_ADD_ORDER_REPLY_T_H
