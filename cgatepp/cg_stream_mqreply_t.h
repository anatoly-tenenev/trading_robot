#ifndef CG_STREAM_MQREPLY_T_H
#define CG_STREAM_MQREPLY_T_H

#include "cg_headers.h"

#include "cg_stream_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_stream_mqreply_t : public cg_stream_base_t
{
public:
    cg_stream_mqreply_t(const string& name, cg_publisher_base_t& publisher);
    virtual ~cg_stream_mqreply_t();

    virtual void send() = 0;

    void process(const ::cg_msg_t* msg) override;

protected:
    cg_publisher_base_t& m_publisher;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_STREAM_MQREPLY_T_H
