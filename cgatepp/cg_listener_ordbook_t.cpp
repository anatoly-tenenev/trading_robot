#include "cg_listener_ordbook_t.h"
#include "cg_stream_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_listener_ordbook_t::cg_listener_ordbook_t(
        cg_stream_t& online_stream,
        cg_stream_t& snapshot_stream,
        cg_connection_t& conn) :

    cg_listener_t(online_stream, snapshot_stream, conn)

{
    create(string("p2ordbook://") + m_online_stream.name() +
           ";snapshot=" + m_snapshot_stream.name() +
           ";scheme=|FILE|ini/" + m_online_stream.short_name() + ".ini|" + m_online_stream.short_name() +
           ";snapshot.scheme=|FILE|ini/" + m_snapshot_stream.short_name() + ".ini|" + m_snapshot_stream.short_name());

    m_messages = m_online_stream.messages();
    auto snapshot_msgs = m_snapshot_stream.messages();
    m_messages.insert(m_messages.end(),
                      snapshot_msgs.begin(),
                      snapshot_msgs.end());
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_listener_ordbook_t::~cg_listener_ordbook_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_ordbook_t::msg_p2repl_online()
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
