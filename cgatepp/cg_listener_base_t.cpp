#include "cg_listener_base_t.h"
#include "cg_connection_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_listener_base_t::cg_listener_base_t(cg_connection_t& conn) :
    m_conn(conn),
    m_cg_lsn(NULL)
{
    m_conn.add_listener(this);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_listener_base_t::~cg_listener_base_t()
{
    if (m_cg_lsn != NULL)
    {
        close();
        destroy();
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_base_t::create(const string &url)
{
    CG_RESULT result;
    result = ::cg_lsn_new(
                    m_conn.cg_object(),
                    url.c_str(),
                    &cg_listener_base_t::message_callback,
                    static_cast<void*>(this),
                    &m_cg_lsn);
    assert(result == CG_ERR_OK);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_base_t::open(const string& settings)
{
    ::cg_lsn_open(m_cg_lsn, settings.c_str());
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_base_t::close()
{
    ::cg_lsn_close(m_cg_lsn);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_base_t::destroy()
{
    ::cg_lsn_destroy(m_cg_lsn);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

CG_RESULT cg_listener_base_t::get_state()
{
    CG_RESULT state = CG_STATE_CLOSED;
    ::cg_lsn_getstate(m_cg_lsn, &state);
    return state;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

CG_RESULT cg_listener_base_t::message_callback(::cg_conn_t* conn, ::cg_listener_t* lsn, ::cg_msg_t* msg, void* data)
{
    cg_listener_base_t* listener = static_cast<cg_listener_base_t*>(data);
    return listener->handle_message(msg);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
