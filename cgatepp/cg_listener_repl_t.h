#ifndef CG_LISTENER_REPL_T_H
#define CG_LISTENER_REPL_T_H

#include "cg_listener_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_listener_repl_t : public cg_listener_t
{
public:
    cg_listener_repl_t(
            cg_stream_t& stream,
            cg_connection_t& conn);
    ~cg_listener_repl_t();
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}



#endif // CG_LISTENER_REPL_T_H
