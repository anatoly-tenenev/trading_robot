#include "cg_publisher_base_t.h"
#include "cg_connection_t.h"
#include "cg_message_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_publisher_base_t::cg_publisher_base_t(const string& name, cg_connection_t& conn) :
    m_name(name),
    m_conn(conn),
    m_cg_pub(NULL)
{
    m_conn.add_publisher(this);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_publisher_base_t::~cg_publisher_base_t()
{
    if (m_cg_pub != NULL)
    {
        close();
        destroy();
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_publisher_base_t::create(const string &url)
{
    //string url = string("p2mq://");
    CG_RESULT result;
    result = ::cg_pub_new(
                    m_conn.cg_object(),
                    url.c_str(),
                    &m_cg_pub);
    assert(result == CG_ERR_OK);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_publisher_base_t::open()
{
    ::cg_pub_open(m_cg_pub, 0);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_publisher_base_t::close()
{
    ::cg_pub_close(m_cg_pub);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_publisher_base_t::destroy()
{
    ::cg_pub_destroy(m_cg_pub);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

CG_RESULT cg_publisher_base_t::get_state()
{
    CG_RESULT state = CG_STATE_CLOSED;
    ::cg_pub_getstate(m_cg_pub, &state);
    return state;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_publisher_base_t::send(cg_message_base_t &msg)
{
    if (m_cg_pub == NULL)
    {
        return;
    }

    ::cg_msg_t* cg_msg;

    auto id = msg.id();

    CG_RESULT result;

    result = ::cg_pub_msgnew(
                m_cg_pub,
                CG_KEY_ID,
                static_cast<void*>(&id),
                &cg_msg);

    if (result != CG_ERR_OK)
    {
        return;
    }

    if (msg.size() != cg_msg->data_size)
    {
        return;
    }

    msg.write(cg_msg->data);

    result = ::cg_pub_post(
                m_cg_pub,
                cg_msg,
                CG_PUB_NEEDREPLY);

    if (result != CG_ERR_OK)
    {
        return;
    }

    result = ::cg_pub_msgfree(
                m_cg_pub,
                cg_msg);

    if (result != CG_ERR_OK)
    {
        return;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


