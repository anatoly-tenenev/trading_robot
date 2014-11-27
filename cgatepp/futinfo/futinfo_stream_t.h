#ifndef FUTINFO_STREAM_T_H
#define FUTINFO_STREAM_T_H

#include "../cg_stream_t.h"
#include "session_msg_t.h"
#include "fut_instruments_msg_t.h"
#include "fut_sess_contents_msg_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class futinfo_stream_t : public cg_stream_t
{
public:
    futinfo_stream_t() :
        cg_stream_t("FORTS_FUTINFO_REPL", "FutInfo")
    {
        to_ini(m_short_name);
    }

    //session_msg_t           session_msg{this};
    fut_instruments_msg_t   fut_instruments_msg{this};
    fut_sess_contents_msg_t fut_sess_contents_msg{this};
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // FUTINFO_STREAM_T_H
