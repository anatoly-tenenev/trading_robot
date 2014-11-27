#include "cg_field_t.h"
#include <cstring>

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<>
void cg_field_t<string>::read(const void* ptr_msg)
{
//    m_data = string(m_size, '\0');
//    const char* str = static_cast<const char*>(ptr_msg)+m_offset;
//    for (size_t i=0; (i < m_size) && (str[i] != '\0'); i++)
//        m_data[i] = str[i];
    m_data.assign(static_cast<const char*>(ptr_msg)+m_offset);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<>
void cg_field_t<double>::read(const void* ptr_msg)
{
    switch (m_type)
    {
    case cg_field_types_t::dNM:
        m_data = bcd_to_double(static_cast<const char*>(ptr_msg)+m_offset);
        break;

    case cg_field_types_t::f:
        m_data = *pointer_cast<double*>(static_cast<const char*>(ptr_msg)+m_offset);
        break;

    default:
        m_data = 0.0;
        break;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<>
void cg_field_t<ptime>::read(const void* ptr_msg)
{
    ::cg_time_t* cg_time = pointer_cast<decltype(cg_time)>(static_cast<const char*>(ptr_msg)+m_offset);
    m_data = ptime(date(cg_time->year, cg_time->month, cg_time->day),
                   time_duration(cg_time->hour, cg_time->minute, cg_time->second, cg_time->msec));
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<>
void cg_field_t<string>::write(void* ptr_msg)
{
    std::strncpy(static_cast<char*>(ptr_msg)+m_offset, m_data.c_str(), m_size);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<>
void cg_field_t<double>::write(void* ptr_msg)
{
    switch (m_type)
    {
    case cg_field_types_t::dNM:
        ::cg_getstr(m_type_name.c_str(), static_cast<void*>(&m_data), static_cast<char*>(ptr_msg)+m_offset, &m_size);
        break;

    case cg_field_types_t::f:
        *pointer_cast<double*>(static_cast<char*>(ptr_msg)+m_offset) = m_data;
        break;

    default:
        *pointer_cast<double*>(static_cast<char*>(ptr_msg)+m_offset) = 0.0;
        break;
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<>
void cg_field_t<ptime>::write(void* ptr_msg)
{
    date d = m_data.date();
    time_duration t = m_data.time_of_day();
    ::cg_time_t* cg_time = pointer_cast<decltype(cg_time)>(static_cast<char*>(ptr_msg)+m_offset);
    cg_time->year = d.year();
    cg_time->month = d.month();
    cg_time->day = d.day();
    cg_time->hour = t.hours();
    cg_time->minute = t.minutes();
    cg_time->second = t.seconds();
    cg_time->msec = t.fractional_seconds();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

template<>
string cg_field_t<string>::def_to_string() const
{
    return string("\"")+m_default+string("\"");
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------


template<>
string cg_field_t<ptime>::def_to_string() const
{
    return string("");
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


