#include "scalper_trade_t.h"
#include "../deal_base_t.h"

namespace strategies
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int scalper_trade_t::s_counter = 0;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void scalper_trade_t::add_deal(const deal_base_t& deal)
{
    if (deal.type() == deal_type_t::buy)
    {
        m_buy_volume += deal.volume();
        m_buy_total += deal.volume() * deal.price();
    }
    else
    {
        m_sell_volume += deal.volume();
        m_sell_total += deal.volume() * deal.price();
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

bool scalper_trade_t::is_ready()
{
    if (m_is_ready)
        return true;

    for (const auto& order : orders)
    {
        if (order.is_pending())
            return false;
    }

    m_is_ready = true;
    return true;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

double scalper_trade_t::cur_profit(double best_ask, double best_bid) const
{
    if (m_sell_total - m_buy_total > 0)
    {
        return (m_sell_total-m_buy_total) - best_bid;
    }
    else if (m_buy_total - m_sell_total > 0)
    {
        return best_ask - (m_buy_total-m_sell_total);
    }
    else
        return 0.0;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
