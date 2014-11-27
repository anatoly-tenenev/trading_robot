#include "deal_log_base_t.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

deal_log_base_t::deal_log_base_t()
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

deal_log_base_t::~deal_log_base_t()
{  
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

vol_t deal_log_base_t::operator[](double price) const
{
    auto it = m_deals.find(price);
    if (it != m_deals.end())
        return it->second;
    else
        return 0;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void deal_log_base_t::set(double price, vol_t volume)
{
    if (volume <= 0)
    {
        m_deals.erase(price);
        if (m_deals.empty())
            m_state = deal_type_t::undefined;
    }
    else
        m_deals[price] = volume;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

vector<double> deal_log_base_t::sells() const
{
    if (m_state == deal_type_t::sell)
    {
        vector<double> sells;
        for (auto it = m_deals.begin(); it != m_deals.end(); it++)
            if (it->second > 0)
                sells.push_back(it->first);
        return sells;
    }
    else
        return vector<double>();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

vector<double> deal_log_base_t::buys() const
{
    if (m_state == deal_type_t::buy)
    {
        vector<double> buys;
        for (auto it = m_deals.rbegin(); it != m_deals.rend(); it++)
            if (it->second > 0)
                buys.push_back(it->first);
        return buys;
    }
    else
        return vector<double>();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void deal_log_base_t::add_deal(const deal_base_t &deal)
{

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

