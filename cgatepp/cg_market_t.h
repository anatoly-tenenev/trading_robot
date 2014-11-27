#ifndef CG_MARKET_T_H
#define CG_MARKET_T_H

#include "cg_headers.h"
#include "../market_base_t.h"

#include "cg_orderbook_t.h"
#include "cg_orderlog_t.h"
#include "cg_deal_log_t.h"

#include "cgate_t.h"
#include "cg_connection_t.h"
#include "cg_robot_t.h"

#include "futinfo/futinfo_stream_t.h"
#include "futtrade/futtrade_stream_t.h"
#include "ordbook/ordbook_stream_t.h"
#include "ordlog/ordlog_stream_t.h"

#include "cg_listener_repl_t.h"
#include "cg_listener_ordbook_t.h"
#include "cg_listener_mqreply_t.h"
#include "cg_publisher_base_t.h"

#include "cg_publisher_base_t.h"
#include "commands/fut_add_order_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_market_t : public trading::market_base_t
{
public:
    cg_market_t();

    trading::robot_base_t& robot() override;
    void run() override;

    void register_symbol(cg_symbol_t& symbol);
    void try_init_symbol(cg_symbol_t& symbol);

    ptime datetime() const { return microsec_clock::local_time(); }

private:
    cg_robot_t m_robot{*this};

    queue<function<void()>> m_tasks;
    vector<cg_symbol_t*> m_symbols_to_init;
    vector<cg_orderlog_t*>  m_orderlogs;
    vector<cg_deal_log_t*>  m_deal_logs;
    vector<cg_orderbook_t*> m_orderbooks;

    cgate_t m_cgate{"ini=robot.ini;key=11111111"};
    cg_connection_t m_conn{"p2tcp://127.0.0.1:4001;app_name=test_repl"};

    futinfo_stream_t futinfo;
    cg_listener_repl_t futinfo_lsn{futinfo, m_conn};

    ordlog_stream_t ordlog;
    ordbook_stream_t ordbook;
    cg_listener_ordbook_t futordlog_lsn{ordlog, ordbook, m_conn};

//    cg_publisher_base_t fut_add_order_pub{"fut_add_order", m_conn};
//    fut_add_order_t fut_add_order{fut_add_order_pub};
//    cg_listener_mqreply_t fut_add_order_lsn{fut_add_order, fut_add_order_pub, m_conn};

    //-------------------------------------------------------------------
    //-------------------------------------------------------------------

    void init_strategy(trading::strategy_base_t* strategy) override;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_MARKET_T_H
