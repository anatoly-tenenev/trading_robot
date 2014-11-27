#ifndef CG_MESSAGE_BASE_T_H
#define CG_MESSAGE_BASE_T_H

#include "cg_headers.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_message_base_t
{
public:
    cg_message_base_t(cg_stream_base_t* stream, const string& name);
    cg_message_base_t(const cg_message_base_t& that) = delete;
    cg_message_base_t& operator=(const cg_message_base_t& that) = delete;
    virtual ~cg_message_base_t();

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

    cg_stream_base_t& stream();

    string name() const { return m_name; }

    void id(msg_id_t id) { m_id = id; }
    msg_id_t id() const { return m_id; }

    void index(size_t index) { m_index = index; }
    size_t index() const { return m_index; }

    void size(size_t size) { m_size = size; }
    size_t size() const { return m_size; }

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

    void add_field(cg_field_base_t* field);
    void add_member(cg_member_base_t* member);

    virtual void read(const ::cg_msg_t* msg);
    void write(void* msg);

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

    void check_fields();
    vector<cg_field_base_t*>& fields() { return m_fields; }

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

    string to_ini(const string& scheme_name) const;

private:
    cg_stream_base_t& m_stream;

protected:
    string m_name;
    msg_id_t m_id;
    size_t m_index;
    size_t m_size;
    vector<cg_field_base_t*> m_fields;
    vector<cg_member_base_t*> m_members;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


#endif // CG_MESSAGE_BASE_T_H
