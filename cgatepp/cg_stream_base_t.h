#ifndef CG_STREAM_BASE_T_H
#define CG_STREAM_BASE_T_H

#include "cg_headers.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_stream_base_t
{
public:
    cg_stream_base_t(const string& name, const string& short_name);
    cg_stream_base_t(const cg_stream_base_t& that) = delete;
    cg_stream_base_t& operator=(const cg_stream_base_t& that) = delete;
    virtual ~cg_stream_base_t();

    string name() const { return m_name; }
    string short_name() const { return m_short_name; }

    void add_message(cg_message_base_t* message);

    void to_ini(const string& filename) const;

    void sync_scheme(const ::cg_scheme_desc_t* cg_scheme_desc);

    virtual void process(const ::cg_msg_t* msg) = 0;

    vector<cg_message_base_t*> messages() const { return m_messages; }

protected:
    string m_name;
    string m_short_name;
    vector<cg_message_base_t*> m_messages;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_STREAM_BASE_T_H
