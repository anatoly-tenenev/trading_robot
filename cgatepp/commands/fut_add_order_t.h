#ifndef FUT_ADD_ORDER_T_H
#define FUT_ADD_ORDER_T_H

#include "../cg_stream_mqreply_t.h"
#include "fut_add_order_request_t.h"
#include "fut_add_order_reply_t.h"
#include "../cg_publisher_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class fut_add_order_t : public cg_stream_mqreply_t
{
public:
    fut_add_order_t(cg_publisher_base_t& publisher) :
        cg_stream_mqreply_t("FORTS_SRV", publisher)
    {
        to_ini(request.name());
    }

    fut_add_order_request_t request{this};
    fut_add_order_reply_t   reply{this};

    void send() override
    {
        m_publisher.send(request);
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // FUT_ADD_ORDER_T_H
