#ifndef ORDBOOK_STREAM_T_H
#define ORDBOOK_STREAM_T_H

#include "../cg_stream_t.h"
#include "orders_msg_t.h"
#include "ordbook_info_msg_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class ordbook_stream_t : public cg_stream_t
{
public:
    ordbook_stream_t() :
        cg_stream_t("FORTS_ORDBOOK_REPL", "Ordbook")
    {
        to_ini(m_short_name);
    }

    orders_msg_t orders_msg{this};
    ordbook_info_msg_t info_msg{this};
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_ORDBOOK_STREAM_T_H
