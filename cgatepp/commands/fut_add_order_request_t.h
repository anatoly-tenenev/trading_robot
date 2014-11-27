#ifndef FUT_ADD_ORDER_REQUEST_T_H
#define FUT_ADD_ORDER_REQUEST_T_H

#include "../cg_message_base_t.h"
#include "../cg_stream_mqreply_t.h"
#include "../cg_field_t.h"
#include "../msg_members/cg_msgid_t.h"
#include "../msg_members/cg_request_t.h"
#include "../msg_members/cg_local_time_field_t.h"
#include "../msg_members/cg_replies_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class fut_add_order_request_t : public cg_message_base_t
{
public:
    fut_add_order_request_t(cg_stream_mqreply_t* stream) :
        cg_message_base_t(stream, "FutAddOrder")
    {
        check_fields();
    }
    ~fut_add_order_request_t() {}

    struct msg_t
    {
        string broker_code;
        string isin;
    };

    msg_t msg;

    cg_field_t<decltype(msg_t::broker_code)>  broker_code{this, "broker_code", "c4", msg.broker_code, ""};
    cg_field_t<decltype(msg_t::isin)>         isin{this, "isin", "c25", msg.isin};

    cg_msgid_t             msgid{this, 36};
    cg_request_t           request{this};
    cg_local_time_field_t  local_time_field{this, "local_stamp"};
    cg_replies_t           replies{this, {101}};

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // FUT_ADD_ORDER_REQUEST_T_H
