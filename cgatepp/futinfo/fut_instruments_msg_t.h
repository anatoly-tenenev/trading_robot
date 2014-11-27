#ifndef FUT_INSTRUMENTS_MSG_T_H
#define FUT_INSTRUMENTS_MSG_T_H

#include "../cg_message_t.h"
#include "../cg_field_t.h"
#include "../msg_members/cg_index_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class fut_instruments_msg_t : public cg_message_t
{
public:
    fut_instruments_msg_t(cg_stream_t* stream) : cg_message_t(stream, "fut_instruments")
    {
        check_fields();
    }

    struct fut_instruments_t
    {
        int64_t replID;
        int64_t replRev;
        int64_t replAct;
        int32_t isin_id;
        string short_isin;
        string isin;
        string name;
        double step_price;
    };

    map<int64_t, fut_instruments_t> table;
    fut_instruments_t msg;

    cg_field_t<decltype(fut_instruments_t::replID)>  f_replID{this, "replID", "i8", msg.replID};
    cg_field_t<decltype(fut_instruments_t::replRev)> f_replRev{this, "replRev", "i8", msg.replRev};
    cg_field_t<decltype(fut_instruments_t::replAct)> f_replAct{this, "replAct", "i8", msg.replAct};
    cg_field_t<decltype(fut_instruments_t::isin_id)> f_isin_id{this, "isin_id", "i4", msg.isin_id};
    cg_field_t<decltype(fut_instruments_t::short_isin)> f_short_isin{this, "short_isin", "c25", msg.short_isin};
    cg_field_t<decltype(fut_instruments_t::isin)> f_isin{this, "isin", "c25", msg.isin};
    cg_field_t<decltype(fut_instruments_t::name)> f_name{this, "name", "c75", msg.name};
    cg_field_t<decltype(fut_instruments_t::step_price)> f_step_price{this, "step_price", "d16.5", msg.step_price};

    cg_index_t index1{this, "IDX_fut_session_ID,UNIQUE,replID"};
    cg_index_t index2{this, "IDX_fut_session_REV_U,UNIQUE,replRev"};

    void update_table() override
    {
        if (msg.replAct != 0)
            table.erase(msg.replID);
        else
            table[msg.replID] = msg;
    }

    void cleardeleted(int64_t replRev) override
    {
        cleardeleted_table(table, replRev);
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // FUT_INSTRUMENTS_MSG_T_H
