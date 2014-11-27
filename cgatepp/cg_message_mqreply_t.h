#ifndef CG_MESSAGE_MQREPLY_T_H
#define CG_MESSAGE_MQREPLY_T_H

#include "cg_headers.h"

#include "cg_message_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_message_mqreply_t : public cg_message_base_t
{
public:
    cg_message_mqreply_t(cg_stream_mqreply_t* stream, const string& name);
    virtual ~cg_message_mqreply_t();

    void read(const ::cg_msg_t* msg) override;

    virtual void event() = 0;

private:
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


#endif // CG_MESSAGE_MQREPLY_T_H
