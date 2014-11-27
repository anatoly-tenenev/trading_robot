#include "cg_stream_t.h"
#include "cg_message_t.h"

#include "cg_utils.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_stream_t::cg_stream_t(const string& name, const string& short_name) :
    cg_stream_base_t(name, short_name)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_stream_t::~cg_stream_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_stream_t::process(const ::cg_msg_t* msg)
{
    auto repl_msg = pointer_cast< ::cg_msg_streamdata_t* >(msg);
    auto msg_index = repl_msg->msg_index;

    assert(msg_index < m_messages.size());

    cg_message_t* message = static_cast<cg_message_t*>( m_messages[msg_index] );
    message->read(msg);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_stream_t::cleardeleted(u_int32_t table_idx, int64_t table_rev)
{
    cg_message_t* message = static_cast<cg_message_t*>( m_messages.at(table_idx) );
    message->cleardeleted(table_rev);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

