#ifndef ORDERS_LOG_MSG_T_H
#define ORDERS_LOG_MSG_T_H

#include "../cg_message_t.h"
#include "../cg_field_t.h"
#include "../msg_members/cg_index_t.h"
#include "../cg_orderbook_t.h"
#include "../cg_symbol_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class orders_log_msg_t : public cg_message_t
{
public:
    orders_log_msg_t(cg_stream_t* stream) : cg_message_t(stream, "orders_log")
    {
        check_fields();
    }

    struct orders_log_t
    {
        int64_t replID;
        int64_t replRev;
        int64_t replAct;
        int64_t id_ord;
        int32_t sess_id;
        ptime moment;
        int32_t status;
        int8_t action;
        int32_t isin_id;
        int8_t dir;
        double price;
        int32_t amount;
        int32_t amount_rest;
        int64_t id_deal;
        double deal_price;
    };

    orders_log_t msg;
    vector<orders_log_t> table;
    bool should_update = true;

    vector<cg_symbol_t*> symbols;
    vector<cg_orderbook_t*> orderbooks;

    cg_field_t<decltype(orders_log_t::replID)>  f_replID{this, "replID", "i8", msg.replID};
    cg_field_t<decltype(orders_log_t::replRev)> f_replRev{this, "replRev", "i8", msg.replRev};
    cg_field_t<decltype(orders_log_t::replAct)> f_replAct{this, "replAct", "i8", msg.replAct};
    cg_field_t<decltype(orders_log_t::id_ord)> f_id_ord{this, "id_ord", "i8", msg.id_ord};
    cg_field_t<decltype(orders_log_t::sess_id)> f_sess_id{this, "sess_id", "i4", msg.sess_id};
    cg_field_t<decltype(orders_log_t::moment)> f_moment{this, "moment", "t", msg.moment};
    cg_field_t<decltype(orders_log_t::status)> f_status{this, "status", "i4", msg.status};
    cg_field_t<decltype(orders_log_t::action)> f_action{this, "action", "i1", msg.action};
    cg_field_t<decltype(orders_log_t::isin_id)> f_isin_id{this, "isin_id", "i4", msg.isin_id};
    cg_field_t<decltype(orders_log_t::dir)> f_dir{this, "dir", "i1", msg.dir};
    cg_field_t<decltype(orders_log_t::price)> f_price{this, "price", "d16.5", msg.price};
    cg_field_t<decltype(orders_log_t::amount)> f_amount{this, "amount", "i4", msg.amount};
    cg_field_t<decltype(orders_log_t::amount_rest)> f_amount_rest{this, "amount_rest", "i4", msg.amount_rest};
    cg_field_t<decltype(orders_log_t::id_deal)> f_id_deal{this, "id_deal", "i8", msg.id_deal};
    cg_field_t<decltype(orders_log_t::deal_price)> f_deal_price{this, "deal_price", "d16.5", msg.deal_price};

    cg_index_t index1{this, "IDX_orders_log_ID,UNIQUE,replID"};
    cg_index_t index2{this, "IDX_orders_log_REV_U,UNIQUE,replRev"};

    void update_table() override
    {
        if (should_update)
            table.push_back(msg);

        std::cout << msg.replID << std::endl;
    }

    void cleardeleted(int64_t replRev) override
    {
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // ORDERS_LOG_MSG_T_H
