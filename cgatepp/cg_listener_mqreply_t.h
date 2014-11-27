#ifndef CG_LISTENER_MQREPLY_T_H
#define CG_LISTENER_MQREPLY_T_H

#include "cg_headers.h"

#include "cg_listener_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_listener_mqreply_t : public cg_listener_base_t
{
public:
    cg_listener_mqreply_t(
            cg_stream_mqreply_t& stream,
            cg_publisher_base_t& publisher,
            cg_connection_t& conn);
    virtual ~cg_listener_mqreply_t();

    void open() override;
    CG_RESULT handle_message(const ::cg_msg_t* msg) override;

private:
    cg_stream_mqreply_t& m_stream;
    cg_publisher_base_t& m_publisher;

    void msg_open();
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_LISTENER_MQREPLY_T_H
