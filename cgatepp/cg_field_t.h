#ifndef CG_FIELD_T_H
#define CG_FIELD_T_H

#include "cg_field_base_t.h"
#include "cg_utils.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<typename T>
class cg_field_t : public cg_field_base_t
{
public:
    cg_field_t(cg_message_base_t* message, const string& name, const string& type_name, T& data) :
        cg_field_base_t(message, name, type_name),
        m_data(data),
        m_default_flag(false)
    {
    }

    cg_field_t(cg_message_base_t* message, const string& name, const string& type_name, T& data, T default_value) :
        cg_field_base_t(message, name, type_name),
        m_data(data),
        m_default_flag(true),
        m_default(default_value)
    {
    }

    ~cg_field_t() {}

    void read(const void* ptr_msg) override
    {
        T* ptr_field = pointer_cast<T*>(static_cast<const char*>(ptr_msg)+m_offset);
        m_data = *ptr_field;
    }

    T data() const { return m_data; }

    void write(void* ptr_msg) override
    {
        T* ptr_field = pointer_cast<T*>(static_cast<char*>(ptr_msg)+m_offset);
        *ptr_field = m_data;
    }

    void set_default() { m_data = m_default; }

    bool default_flag() const { return m_default_flag; }

    string to_ini() const override
    {
        stringstream result;
        result << "field=" << m_name << "," << m_type_name;
        if (m_default_flag)
            result << ",," << def_to_string();
        return result.str();
    }

protected:
    T& m_data;
    bool m_default_flag = false;
    T m_default;

private:
    string def_to_string() const { return std::to_string(m_data); }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<>
void cg_field_t<string>::read(const void* ptr_msg);

template<>
void cg_field_t<double>::read(const void* ptr_msg);

template<>
void cg_field_t<ptime>::read(const void* ptr_msg);

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<>
void cg_field_t<string>::write(void* ptr_msg);

template<>
void cg_field_t<double>::write(void* ptr_msg);

template<>
void cg_field_t<ptime>::write(void* ptr_msg);

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<>
string cg_field_t<string>::def_to_string() const;

template<>
string cg_field_t<ptime>::def_to_string() const;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_FIELD_T_H
