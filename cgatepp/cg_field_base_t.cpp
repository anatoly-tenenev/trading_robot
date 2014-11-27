#include "cg_field_base_t.h"
#include "cg_message_t.h"

#include "cg_utils.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_field_base_t::cg_field_base_t(cg_message_base_t* message, const string& name, const string& type_name) :
    m_name(name),
    m_type_name(type_name)
{
    handle_type();
    message->add_field(this);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_field_base_t::~cg_field_base_t()
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_field_base_t::handle_type()
{
    if (m_type_name == "i1")
    {
        m_type = cg_field_types_t::i1;
        m_size = sizeof(int8_t);
    }
    else if (m_type_name == "i2")
    {
        m_type = cg_field_types_t::i2;
        m_size = sizeof(int16_t);
    }
    else if (m_type_name == "i4")
    {
        m_type = cg_field_types_t::i4;
        m_size = sizeof(int32_t);
    }
    else if (m_type_name == "i8")
    {
        m_type = cg_field_types_t::i8;
        m_size = sizeof(int64_t);
    }
    else if (m_type_name == "t")
    {
        m_type = cg_field_types_t::t;
        m_size = sizeof(::cg_time_t);
    }
    else if (m_type_name == "f")
    {
        m_type = cg_field_types_t::f;
        m_size = sizeof(double);
    }
    else if (m_type_name[0] == 'c')
    {
        m_type = cg_field_types_t::cN;
        std::string t_s = m_type_name;
        t_s.replace(0,1,"");
        cN_N = std::stoul(t_s);
        assert(m_type_name == "c"+std::to_string(cN_N));
        m_size = cN_N + 1;
    }
    else if (m_type_name[0] == 'd')
    {
        m_type = cg_field_types_t::dNM;
        std::string t_s = m_type_name;
        t_s.replace(0,1,"");
        size_t point = t_s.find(".");
        dNM_N = std::stoul( t_s.substr(0, point) );
        dNM_M = std::stoul( t_s.substr(point+1) );
        assert(m_type_name == "d"+std::to_string(dNM_N)+"."+std::to_string(dNM_M));
        m_size = bcd_len(dNM_N, dNM_M);
    }
    else
    {
        assert(false);
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
