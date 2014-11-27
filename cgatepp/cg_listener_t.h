#ifndef CG_LISTENER_T_H
#define CG_LISTENER_T_H

#include "cg_headers.h"

#include "cg_listener_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_listener_t : public cg_listener_base_t
{
public:
    cg_listener_t(
            cg_stream_t& online_stream,
            cg_stream_t& snapshot_stream,
            cg_connection_t& conn);
    virtual ~cg_listener_t();

    void open() override;
    CG_RESULT handle_message(const ::cg_msg_t* msg) override;

    bool is_commit() const { return m_commit; }
    bool is_msg_commit() const { return m_msg_commit; }
    bool is_online() const { return m_online; }

protected:
    cg_stream_t& m_online_stream;
    cg_stream_t& m_snapshot_stream;
    vector<cg_message_base_t*> m_messages;

    bool m_commit = false;
    bool m_msg_commit = false;
    bool m_online = false;
    string m_replstate = "";
    u_int64_t m_lifenum = 0;

    void msg_open();
    void sync_scheme(const ::cg_scheme_desc_t* cg_scheme_desc);
    void msg_stream_data(const ::cg_msg_t* msg);
    virtual void msg_p2repl_online();
    void msg_lifenum(const ::cg_msg_t* msg);
    void msg_cleardeleted(const ::cg_msg_t* msg);
    void msg_replstate(const ::cg_msg_t* msg);
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_LISTENER_T_H
