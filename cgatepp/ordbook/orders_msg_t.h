#ifndef ORDERS_MSG_T_H
#define ORDERS_MSG_T_H

#include "../cg_message_t.h"
#include "../cg_field_t.h"
#include "../msg_members/cg_index_t.h"
#include "../cg_orderbook_t.h"
#include "../cg_symbol_t.h"

#include <iostream>

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class orders_msg_t : public cg_message_t
{
public:
    orders_msg_t(cg_stream_t* stream) : cg_message_t(stream, "orders")
    {
        check_fields();
    }

    struct orders_t
    {
        int64_t replID;
        int64_t replRev;
        int64_t replAct;
        int64_t id_ord;
        int32_t sess_id;
        string client_code;
        ptime moment;
        int32_t status;
        int8_t action;
        int32_t isin_id;
        int8_t dir;
        double price;
        int32_t amount;
        int32_t amount_rest;
        ptime init_moment;
        int32_t init_amount;
    };

    orders_t msg;
    vector<orders_t> table;
    bool should_update = true;

    vector<cg_symbol_t*> symbols;
    vector<cg_orderbook_t*> orderbooks;

    cg_field_t<decltype(orders_t::replID)>  f_replID{this, "replID", "i8", msg.replID};
    cg_field_t<decltype(orders_t::replRev)> f_replRev{this, "replRev", "i8", msg.replRev};
    cg_field_t<decltype(orders_t::replAct)> f_replAct{this, "replAct", "i8", msg.replAct};
    cg_field_t<decltype(orders_t::id_ord)> f_id_ord{this, "id_ord", "i8", msg.id_ord};
    cg_field_t<decltype(orders_t::sess_id)> f_sess_id{this, "sess_id", "i4", msg.sess_id};
    cg_field_t<decltype(orders_t::client_code)> f_client_code{this, "client_code", "c7", msg.client_code};
    cg_field_t<decltype(orders_t::moment)> f_moment{this, "moment", "t", msg.moment};
    cg_field_t<decltype(orders_t::status)> f_status{this, "status", "i4", msg.status};
    cg_field_t<decltype(orders_t::action)> f_action{this, "action", "i1", msg.action};
    cg_field_t<decltype(orders_t::isin_id)> f_isin_id{this, "isin_id", "i4", msg.isin_id};
    cg_field_t<decltype(orders_t::dir)> f_dir{this, "dir", "i1", msg.dir};
    cg_field_t<decltype(orders_t::price)> f_price{this, "price", "d16.5", msg.price};
    cg_field_t<decltype(orders_t::amount)> f_amount{this, "amount", "i4", msg.amount};
    cg_field_t<decltype(orders_t::amount_rest)> f_amount_rest{this, "amount_rest", "i4", msg.amount_rest};
    cg_field_t<decltype(orders_t::init_moment)> f_init_moment{this, "init_moment", "t", msg.init_moment};
    cg_field_t<decltype(orders_t::init_amount)> f_init_amount{this, "init_amount", "i4", msg.init_amount};

    cg_index_t index1{this, "IDX_orders_ID,UNIQUE,replID"};
    cg_index_t index2{this, "IDX_orders_REV_U,UNIQUE,replRev"};

    void update_table() override
    {
        if (should_update)
            table.push_back(msg);

        //std::cout << msg.replID << std::endl;
    }

    void cleardeleted(int64_t replRev) override
    {
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // ORDERS_MSG_T_H
