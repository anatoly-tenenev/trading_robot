#ifndef CG_LISTENER_BASE_T_H
#define CG_LISTENER_BASE_T_H

#include "cg_headers.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_listener_base_t
{
public:
    cg_listener_base_t(cg_connection_t& conn);
    cg_listener_base_t(const cg_listener_base_t& that) = delete;
    cg_listener_base_t& operator=(const cg_listener_base_t& that) = delete;
    virtual ~cg_listener_base_t();

    void create(const string& url);
    virtual void open() = 0;
    void open(const string& settings);
    void close();
    void destroy();
    CG_RESULT get_state();

    static CG_RESULT message_callback(::cg_conn_t* conn, ::cg_listener_t* lsn, ::cg_msg_t* msg, void* data);
    virtual CG_RESULT handle_message(const ::cg_msg_t* msg) = 0;

protected:
    cg_connection_t& m_conn;
    ::cg_listener_t* m_cg_lsn;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


#endif // CG_LISTENER_BASE_T_H
