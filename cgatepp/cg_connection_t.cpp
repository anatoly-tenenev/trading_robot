#include "cg_connection_t.h"
#include "cg_listener_base_t.h"
#include "cg_publisher_base_t.h"

#include <chrono>
#include <thread>

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_connection_t::cg_connection_t(const string& settings) :
    m_cg_conn(NULL)
{
    CG_RESULT result;
    result = ::cg_conn_new(settings.c_str(), &m_cg_conn);
    assert(result == CG_ERR_OK);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_connection_t::~cg_connection_t()
{
    if (m_cg_conn != NULL)
    {
        close();
        destroy();
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_connection_t::open()
{
    CG_RESULT result;
    result = ::cg_conn_open(m_cg_conn, 0);
    if (result != CG_ERR_OK)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
        ++conn_attempts;
        if (conn_attempts >= 100000)
        {
            std::exit(EXIT_FAILURE);
        }
    }
    else
    {
        conn_attempts = 0;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_connection_t::close()
{
    ::cg_conn_close(m_cg_conn);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_connection_t::destroy()
{
    ::cg_conn_destroy(m_cg_conn);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

CG_RESULT cg_connection_t::get_state()
{
    CG_RESULT state = CG_STATE_CLOSED;
    ::cg_conn_getstate(m_cg_conn, &state);
    return state;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_connection_t::process()
{
    ::cg_conn_process(m_cg_conn, 1, 0);

    for (auto& pub : m_pubs)
    {
        switch (pub->get_state())
        {
        case CG_STATE_CLOSED:
            pub->open();
            break;

        case CG_STATE_ERROR:
            pub->close();
            break;

        default:
            break;
        }
    }

    for (auto& lsn : m_lsns)
    {
        switch (lsn->get_state())
        {
        case CG_STATE_CLOSED:
            lsn->open();
            break;

        case CG_STATE_ERROR:
            lsn->close();
            break;

        default:
            break;
        }
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_connection_t::add_listener(cg_listener_base_t* lsn)
{
    m_lsns.push_back(lsn);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_connection_t::add_publisher(cg_publisher_base_t* pub)
{
    m_pubs.push_back(pub);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
