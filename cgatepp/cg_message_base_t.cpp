#include "cg_message_base_t.h"
#include "cg_stream_base_t.h"
#include "cg_field_base_t.h"
#include "cg_member_base_t.h"

#include "cg_utils.h"
#include "unordered_set"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_message_base_t::cg_message_base_t(cg_stream_base_t* stream, const string& name) :
    m_stream(*stream),
    m_name{name}
{
    m_stream.add_message(this);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_message_base_t::~cg_message_base_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_stream_base_t& cg_message_base_t::stream()
{
    return m_stream;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_message_base_t::add_field(cg_field_base_t* field)
{
    m_fields.push_back(field);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_message_base_t::add_member(cg_member_base_t* member)
{
    m_members.push_back(member);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_message_base_t::read(const ::cg_msg_t* msg)
{
    auto repl_msg = pointer_cast< ::cg_msg_streamdata_t* >(msg);

    assert(m_size == repl_msg->data_size);
    assert(m_id == repl_msg->msg_id);
    assert(m_index == repl_msg->msg_index);

    for (auto& field : m_fields)
    {
        field->read(repl_msg->data);
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_message_base_t::write(void *msg)
{
    for (auto& field : m_fields)
    {
        field->write(msg);
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_message_base_t::check_fields()
{
    std::unordered_set<void*> field_addr;
    for (auto& field : m_fields)
    {
        field_addr.insert( static_cast<void*>(&field) );
    }
    assert(m_fields.size() > 0);
    assert(field_addr.size() == m_fields.size());
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

string cg_message_base_t::to_ini(const string& scheme_name) const
{
    stringstream result;
    result << "[table:" << scheme_name
           << ":" << m_name << "]";
    for (const auto& field : m_fields)
        result << "\n" << field->to_ini();
    result << "\n";
    for (const auto& member : m_members)
        result << "\n" << member->to_ini();
    return result.str();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

