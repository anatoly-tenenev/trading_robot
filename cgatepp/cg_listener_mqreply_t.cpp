#include "cg_listener_mqreply_t.h"
#include "cg_publisher_base_t.h"
#include "cg_stream_mqreply_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_listener_mqreply_t::cg_listener_mqreply_t(
        cg_stream_mqreply_t& stream,
        cg_publisher_base_t& publisher,
        cg_connection_t& conn) :

    cg_listener_base_t(conn),
    m_stream(stream),
    m_publisher(publisher)

{
    create(string("p2mqreply://;ref=") + m_publisher.name());
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_listener_mqreply_t::~cg_listener_mqreply_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_mqreply_t::open()
{
    if (m_publisher.get_state() == CG_STATE_ACTIVE)
        cg_listener_base_t::open("");
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

CG_RESULT cg_listener_mqreply_t::handle_message(const ::cg_msg_t* msg)
{
    switch (msg->type)
    {
    case CG_MSG_OPEN:
        msg_open();
        break;

    case CG_MSG_CLOSE:
        break;

    case CG_MSG_DATA:
        m_stream.process(msg);
        break;

    case CG_MSG_P2MQ_TIMEOUT:
        break;

    default:
        break;
    }

    return CG_ERR_OK;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_mqreply_t::msg_open()
{
    ::cg_scheme_desc_t* cg_scheme_desc;
    CG_RESULT result = ::cg_pub_getscheme(m_publisher.cg_object(), &cg_scheme_desc);
    assert(result == CG_ERR_OK);
    m_stream.sync_scheme(cg_scheme_desc);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

