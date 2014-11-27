#ifndef BT_MARKETDATA_T_H
#define BT_MARKETDATA_T_H

#include "bt_headers.h"
#include "../marketdata_base_t.h"
#include "bt_orderlog_t.h"
#include "bt_orderbook_t.h"
#include "bt_deal_log_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class bt_marketdata_t : public trading::marketdata_base_t
{
public:
    bt_marketdata_t();

    trading::orderlog_base_t& orderlog() override;
    trading::orderbook_base_t& orderbook() override;
    trading::deal_log_base_t& deal_log() override;

private:
    bt_orderlog_t m_orderlog;
    bt_orderbook_t m_orderbook;
    bt_deal_log_t m_deal_log;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_MARKETDATA_T_H
