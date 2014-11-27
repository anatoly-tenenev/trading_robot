#ifndef ORDLOG_STREAM_T_H
#define ORDLOG_STREAM_T_H

#include "../cg_stream_t.h"
#include "orders_log_msg_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class ordlog_stream_t : public cg_stream_t
{
public:
    ordlog_stream_t() :
        cg_stream_t("FORTS_ORDLOG_REPL", "Ordlog")
    {
        to_ini(m_short_name);
    }

    orders_log_msg_t orders_log_msg{this};
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_ORDLOG_STREAM_T_H
