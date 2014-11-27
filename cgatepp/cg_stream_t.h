#ifndef CG_STREAM_T_H
#define CG_STREAM_T_H

#include "cg_headers.h"

#include "cg_stream_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_stream_t : public cg_stream_base_t
{
public:
    cg_stream_t(const string& name, const string& short_name);
    virtual ~cg_stream_t();

    void process(const ::cg_msg_t* msg) override;

    void cleardeleted(u_int32_t table_idx, int64_t table_rev);

private:


};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_STREAM_T_H
