#ifndef CG_FIELD_BASE_T_H
#define CG_FIELD_BASE_T_H

#include "cg_headers.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_field_base_t
{
public:
    cg_field_base_t(cg_message_base_t* message, const string& name, const string& type_name);
    cg_field_base_t(const cg_field_base_t& that) = delete;
    cg_field_base_t& operator=(const cg_field_base_t& that) = delete;
    virtual ~cg_field_base_t();

    virtual void read(const void* ptr_msg) = 0;
    virtual void write(void* ptr_msg) = 0;

    string name() const { return m_name; }
    string type_name() const { return m_type_name; }
    size_t size() const { return m_size; }

    void offset(size_t offset) { m_offset = offset; }
    size_t offset() const { return m_offset; }

    virtual string to_ini() const = 0;

protected:
    string m_name;
    string m_type_name;
    cg_field_types_t m_type;
    size_t m_size;
    size_t m_offset;
    size_t cN_N;
    size_t dNM_N;
    size_t dNM_M;

    void handle_type();
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_FIELD_BASE_T_H
