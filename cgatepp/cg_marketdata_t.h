#ifndef CG_MARKETDATA_T_H
#define CG_MARKETDATA_T_H

#include "cg_headers.h"
#include "../marketdata_base_t.h"
#include "cg_orderlog_t.h"
#include "cg_orderbook_t.h"
#include "cg_deal_log_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_marketdata_t : public trading::marketdata_base_t
{
public:
    cg_marketdata_t();

    trading::orderlog_base_t& orderlog() override;
    trading::orderbook_base_t& orderbook() override;
    trading::deal_log_base_t& deal_log() override;

private:
    cg_orderlog_t m_orderlog;
    cg_orderbook_t m_orderbook;
    cg_deal_log_t m_deal_log;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_MARKETDATA_T_H
