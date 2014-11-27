#ifndef CG_LISTENER_ORDBOOK_T_H
#define CG_LISTENER_ORDBOOK_T_H

#include "cg_listener_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_listener_ordbook_t : public cg_listener_t
{
public:
    cg_listener_ordbook_t(
            cg_stream_t& online_stream,
            cg_stream_t& snapshot_stream,
            cg_connection_t& conn);
    ~cg_listener_ordbook_t();

    void msg_p2repl_online() override;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_LISTENER_ORDBOOK_T_H
