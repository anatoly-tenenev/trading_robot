#ifndef CG_PUBLISHER_BASE_T_H
#define CG_PUBLISHER_BASE_T_H

#include "cg_headers.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_publisher_base_t
{
public:
    cg_publisher_base_t(const string& name, cg_connection_t& conn);
    cg_publisher_base_t(const cg_publisher_base_t& that) = delete;
    cg_publisher_base_t& operator=(const cg_publisher_base_t& that) = delete;
    virtual ~cg_publisher_base_t();

    ::cg_publisher_t* cg_object() { return m_cg_pub; }

    void create(const string& url);
    void open();
    void close();
    void destroy();
    CG_RESULT get_state();
    void send(cg_message_base_t& msg);

    void name(const string& name) { m_name = name; }
    string name() const { return m_name; }

protected:
    string m_name;
    cg_connection_t& m_conn;
    ::cg_publisher_t* m_cg_pub;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}



#endif // CG_PUBLISHER_BASE_T_H
