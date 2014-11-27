#include "cg_listener_repl_t.h"
#include "cg_stream_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_listener_repl_t::cg_listener_repl_t(
        cg_stream_t &stream,
        cg_connection_t &conn) :

    cg_listener_t(stream, stream, conn)

{
    create(string("p2repl://") + m_online_stream.name() +
           ";scheme=|FILE|ini/" + m_online_stream.short_name() + ".ini|" + m_online_stream.short_name());

    m_messages = m_online_stream.messages();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_listener_repl_t::~cg_listener_repl_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


