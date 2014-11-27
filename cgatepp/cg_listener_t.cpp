#include "cg_listener_t.h"
#include "cg_stream_t.h"
#include "cg_message_base_t.h"
#include "cg_message_t.h"
#include "cg_field_base_t.h"
#include "cg_utils.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_listener_t::cg_listener_t(cg_stream_t& online_stream,
                             cg_stream_t& snapshot_stream,
                             cg_connection_t& conn) :
    cg_listener_base_t(conn),
    m_online_stream(online_stream),
    m_snapshot_stream(snapshot_stream)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_listener_t::~cg_listener_t()
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void
cg_listener_t::open()
{
    vector<string> settings{};
    if (!m_replstate.empty())
        settings.push_back("replstate="+m_replstate);
    else
    {
        if (m_lifenum != 0)
            settings.push_back("lifenum="+m_lifenum);
    }

    std::stringstream settings_str;
    for(size_t i = 0; i < settings.size(); ++i)
    {
      if(i != 0)
        settings_str << ";";
      settings_str << settings[i];
    }

    cg_listener_base_t::open(settings_str.str());
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

CG_RESULT cg_listener_t::handle_message(const ::cg_msg_t* msg)
{
    m_msg_commit = false;

    switch (msg->type)
    {
    case CG_MSG_OPEN:
        msg_open();
        break;

    case CG_MSG_CLOSE:
        m_online = false;
        break;

    case CG_MSG_TN_BEGIN:
        m_commit = false;
        break;

    case CG_MSG_TN_COMMIT:
        m_commit = true;
        m_msg_commit = true;
        break;

    case CG_MSG_STREAM_DATA:
        msg_stream_data(msg);
        break;

    case CG_MSG_P2REPL_ONLINE:
        m_online = true;
        msg_p2repl_online();
        break;

    case CG_MSG_P2REPL_LIFENUM:
        msg_lifenum(msg);
        break;

    case CG_MSG_P2REPL_CLEARDELETED:
        msg_cleardeleted(msg);
        break;

    case CG_MSG_P2REPL_REPLSTATE:
        msg_replstate(msg);
        break;

    default:
        break;
    }

    return CG_ERR_OK;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_t::msg_open()
{
    ::cg_scheme_desc_t* cg_scheme_desc;
    CG_RESULT result = ::cg_lsn_getscheme(m_cg_lsn, &cg_scheme_desc);
    assert(result == CG_ERR_OK);
    sync_scheme(cg_scheme_desc);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_t::sync_scheme(const ::cg_scheme_desc_t* cg_scheme_desc)
{
    if (m_messages.size() != cg_scheme_desc->num_messages)
    {
        std::cout << "m_messages.size() != cg_scheme_desc->num_messages" << std::endl
                  << m_messages.size() << " != " << cg_scheme_desc->num_messages << std::endl;
        ::cg_message_desc_t* msg_desc = cg_scheme_desc->messages;
        for (size_t mid = 0; mid < cg_scheme_desc->num_messages; msg_desc = msg_desc->next, mid++)
            std::cout << msg_desc->name << std::endl;
        assert(m_messages.size() == cg_scheme_desc->num_messages);
    }

    map<string, msg_id_t> msg_id;
    map<string, size_t> msg_index;
    map<string, size_t> msg_size;
    map< string, map<string, string> > field_type_name;
    map< string, map<string, size_t> > field_size;
    map< string, map<string, size_t> > field_offset;

    ::cg_message_desc_t* msg_desc = cg_scheme_desc->messages;
    for (size_t mid = 0; mid < cg_scheme_desc->num_messages; msg_desc = msg_desc->next, mid++)
    {
        msg_index[msg_desc->name] = mid;
        msg_id[msg_desc->name] = msg_desc->id;
        msg_size[msg_desc->name] = msg_desc->size;

        ::cg_field_desc_t* field_desc = msg_desc->fields;
        for (size_t fid = 0; fid < msg_desc->num_fields; field_desc = field_desc->next, fid++)
        {
            field_type_name[msg_desc->name][field_desc->name] = field_desc->type;
            field_size[msg_desc->name][field_desc->name] = field_desc->size;
            field_offset[msg_desc->name][field_desc->name] = field_desc->offset;
        }
    }

    for (auto& msg : m_messages)
    {
        msg->id( msg_id.at(msg->name()) );
        msg->index( msg_index.at(msg->name()) );
        msg->size( msg_size.at(msg->name()) );

        for(auto& field : msg->fields())
        {
            field_type_name.at(msg->name());
            field_type_name.at(msg->name()).at(field->name());
            assert( field->type_name() == field_type_name.at(msg->name()).at(field->name()) );
            assert( field->size() == field_size.at(msg->name()).at(field->name()) );

            field->offset( field_offset.at(msg->name()).at(field->name()) );
        }
    }

    std::sort( m_messages.begin(), m_messages.end(), msg_by_index_compare);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_t::msg_stream_data(const ::cg_msg_t* msg)
{
    auto repl_msg = pointer_cast< ::cg_msg_streamdata_t* >(msg);
    auto msg_index = repl_msg->msg_index;

    assert(msg_index < m_messages.size());

    m_messages[msg_index]->read(msg);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_t::msg_p2repl_online()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_t::msg_lifenum(const ::cg_msg_t* msg)
{
    size_t data_size = msg->data_size;
    switch (data_size)
    {
    case 1:
        m_lifenum = static_cast<u_int64_t>( *static_cast<u_int8_t*>(msg->data) );
        break;

    case 2:
        m_lifenum = static_cast<u_int64_t>( *static_cast<u_int16_t*>(msg->data) );
        break;

    case 4:
        m_lifenum = static_cast<u_int64_t>( *static_cast<u_int32_t*>(msg->data) );
        break;

    case 8:
        m_lifenum = static_cast<u_int64_t>( *static_cast<u_int64_t*>(msg->data) );
        break;

    default:
        m_lifenum = 0;
        break;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_t::msg_cleardeleted(const ::cg_msg_t* msg)
{
    auto data = static_cast< ::cg_data_cleardeleted_t* >(msg->data);

    assert(data->table_idx < m_messages.size());

    auto message = static_cast<cg_message_t*>(m_messages[data->table_idx]);
    message->cleardeleted(data->table_rev);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_listener_t::msg_replstate(const ::cg_msg_t* msg)
{
    m_replstate.assign(static_cast<const char*>(msg->data));
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
