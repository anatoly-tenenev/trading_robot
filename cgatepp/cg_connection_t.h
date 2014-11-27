#ifndef CG_CONNECTION_T_H
#define CG_CONNECTION_T_H

#include "cg_headers.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_connection_t
{
public:
    cg_connection_t(const string& settings);
    cg_connection_t(const cg_connection_t& that) = delete;
    cg_connection_t& operator=(const cg_connection_t& that) = delete;
    ~cg_connection_t();
    void open();
    void close();
    void destroy();
    CG_RESULT get_state();
    ::cg_conn_t* cg_object() { return m_cg_conn; }
    void process();
    void add_listener(cg_listener_base_t* lsn);
    void add_publisher(cg_publisher_base_t* pub);

private:
    ::cg_conn_t* m_cg_conn;
    vector<cg_listener_base_t*> m_lsns;
    vector<cg_publisher_base_t*> m_pubs;
    size_t timeout = 5000;
    size_t conn_attempts = 0;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_CONNECTION_T_H
