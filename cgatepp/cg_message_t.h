#ifndef CG_MESSAGE_T_H
#define CG_MESSAGE_T_H

#include "cg_headers.h"

#include "cg_message_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_message_t : public cg_message_base_t
{
public:
    cg_message_t(cg_stream_t* stream, const string& name);
    virtual ~cg_message_t();

    void read(const ::cg_msg_t* msg) override;

    virtual void update_table() = 0;

    virtual void cleardeleted(int64_t replRev) = 0;

    template<typename T>
    static void cleardeleted_table(T& table, int64_t replRev)
    {
        for (auto it = table.begin(); it != table.end(); )
        {
            auto& row = it->second;
            if (row.replRev <= replRev)
                it = table.erase(it);
            else
                ++it;
        }
    }

protected:

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_MESSAGE_T_H
