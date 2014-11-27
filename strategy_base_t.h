#ifndef STRATEGY_BASE_T_H
#define STRATEGY_BASE_T_H

#include "headers.h"
#include "trade_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class strategy_base_t
{
public:
    strategy_base_t(
            market_base_t& market,
            symbol_base_t& symbol,
            marketdata_base_t& marketdata);
    ~strategy_base_t();

    virtual void process() = 0;

    //static void order_succeeded_callback();
    virtual void order_succeeded(const order_base_t& order) = 0;

    //static void order_failed_callback();
    virtual void order_failed(const order_base_t& order) = 0;

    //static void order_del_succeeded_callback();
    virtual void order_del_succeeded(const order_base_t& order) = 0;

    //static void order_del_failed_callback();
    virtual void order_del_failed(const order_base_t& order) = 0;

    //static void order_move_succeeded_callback();
    virtual void order_move_succeeded(const order_base_t& order) = 0;

    //static void order_move_failed_callback();
    virtual void order_move_failed(const order_base_t& order) = 0;

    virtual void add_deal(const deal_base_t& deal) = 0;
    virtual void add_tick(const deal_base_t& tick) = 0;

    symbol_base_t& symbol() { return m_symbol; }
    orderlog_base_t& orderlog() { return m_orderlog; }
    deal_log_base_t& deal_log() { return m_deal_log; }
    orderbook_base_t& orderbook() { return m_orderbook; }

    void add_trade(const trade_t& trade);
    virtual void update_orders_before();
    virtual void update_orderbook();

    double result() const { return m_tradelog.total(); }
    const tradelog_t& tradelog() const { return m_tradelog; }

protected:
    market_base_t& m_market;
    robot_base_t& m_robot;
    symbol_base_t& m_symbol;
    orderlog_base_t& m_orderlog;
    orderbook_base_t& m_orderbook;
    deal_log_base_t& m_deal_log;
    tradelog_t m_tradelog;

    void add_order(const order_base_t& order);
    void del_order(const order_base_t& order);
    void move_order(const order_base_t& order, double price);
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}



#endif // STRATEGY_BASE_T_H
