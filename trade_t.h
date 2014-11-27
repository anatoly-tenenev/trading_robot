#ifndef TRADE_T_H
#define TRADE_T_H

#include "headers.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

struct trade_t
{
    double profit;
    ptime datetime;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

//typedef vector<trade_t> trade_log_t;

class tradelog_t
{
public:
    vector<trade_t> trades;

    void add(const trade_t& trade)
    {
        trades.push_back(trade);
        m_total += trade.profit;
        if (trade.profit > 0)
        {
            m_pos_trades++;
            m_pos_total += trade.profit;
        }
        else if (trade.profit < 0)
        {
            m_neg_trades++;
            m_neg_total += -trade.profit;
        }
    }

    double total() const { return m_total; }
    double pos_total() const { return m_pos_total; }
    double neg_total() const { return m_neg_total; }

    int pos_trades() const { return m_pos_trades; }
    int neg_trades() const { return m_neg_trades; }

protected:
    double m_total = 0.0;
    int m_pos_trades = 0;
    double m_pos_total = 0.0;
    int m_neg_trades = 0;
    double m_neg_total = 0;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // TRADE_T_H
