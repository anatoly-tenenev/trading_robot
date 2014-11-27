#include "cg_stream_base_t.h"
#include "cg_message_base_t.h"
#include "cg_field_base_t.h"

#include "cg_utils.h"

#include <boost/filesystem.hpp>

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_stream_base_t::cg_stream_base_t(const string& name, const string& short_name) :
    m_name{name},
    m_short_name{short_name}
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_stream_base_t::~cg_stream_base_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_stream_base_t::add_message(cg_message_base_t* message)
{
    m_messages.push_back(message);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_stream_base_t::to_ini(const string& filename) const
{
    boost::filesystem::path dir("ini");
    if(!(boost::filesystem::exists(dir)))
        boost::filesystem::create_directory(dir);

    std::ofstream file{string("ini/") + filename + ".ini"};
    file << "[dbscheme:" << m_short_name << "]\n";
    for (const auto& msg : m_messages)
        file << "table=" << msg->name() << "\n";
    file << "\n";
    for (const auto& msg : m_messages)
        file << msg->to_ini(m_short_name) << "\n\n";
    file << "\n";
    file.flush();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_stream_base_t::sync_scheme(const ::cg_scheme_desc_t* cg_scheme_desc)
{
//    if (m_messages.size() != cg_scheme_desc->num_messages)
//    {
//        std::cout << "m_messages.size() != cg_scheme_desc->num_messages" << std::endl
//                  << m_name << ": " << m_messages.size() << " != " << cg_scheme_desc->num_messages
//                  << std::endl;
//        ::cg_message_desc_t* msg_desc = cg_scheme_desc->messages;
//        for (size_t mid = 0; mid < cg_scheme_desc->num_messages; msg_desc = msg_desc->next, mid++)
//            std::cout << msg_desc->name << std::endl;
//        assert(m_messages.size() == cg_scheme_desc->num_messages);
//    }

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
            assert( field->type_name() == field_type_name.at(msg->name()).at(field->name()) );
            assert( field->size() == field_size.at(msg->name()).at(field->name()) );

            field->offset( field_offset.at(msg->name()).at(field->name()) );
        }
    }

    std::sort( m_messages.begin(), m_messages.end(), msg_by_index_compare);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


