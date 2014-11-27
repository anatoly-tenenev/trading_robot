#ifndef ADD_DEAL_HPP
#define ADD_DEAL_HPP

#include "../headers.h"

namespace trading {
namespace functions {

//----------------------------------------------------------------
//----------------------------------------------------------------

using namespace trading;

//----------------------------------------------------------------
//----------------------------------------------------------------

template<typename DealT, typename DealLogT>
tuple< DealLogT, vector<double> >
add_buy_deal(const DealT& deal, const DealLogT& _deal_log);

template<typename DealT, typename DealLogT>
tuple< DealLogT, vector<double> >
add_sell_deal(const DealT& deal, const DealLogT& _deal_log);

//----------------------------------------------------------------
//----------------------------------------------------------------

template<typename DealT, typename DealLogT>
tuple< DealLogT, vector<double> >
add_deal(const DealT& deal, const DealLogT& deal_log)
{
    switch (deal.type())
    {
    case deal_type_t::buy:
        return add_buy_deal(deal, deal_log);
    case deal_type_t::sell:
        return add_sell_deal(deal, deal_log);
    default:
        return make_tuple(deal_log, vector<double>());
    }
}

//----------------------------------------------------------------
//----------------------------------------------------------------

template<typename DealT, typename DealLogT>
tuple< DealLogT, vector<double> >
add_buy_deal(const DealT& deal, const DealLogT& _deal_log)
{
    DealLogT deal_log{_deal_log};
    const auto deal_price = deal.price();
    const auto deal_volume = deal.volume();

    vector<double> profits;

    if ((deal_log.state() == deal_type_t::buy) || (deal_log.state() == deal_type_t::undefined))
    {
        deal_log.set(deal_price,
                     deal_log[deal_price] + deal_volume);
        deal_log.state(deal_type_t::buy);
    }
    else
    {
        auto volume = deal_volume;
        for (double price : deal_log.sells())
        {
            if (volume <= deal_log[price])
            {
                profits.push_back( volume * (price - deal_price) );
                deal_log.set(price,
                             deal_log[price] - volume);
                volume = 0;
                break;
            }
            else
            {
                profits.push_back( deal_log[price] * (price - deal_price) );
                volume -= deal_log[price];
                deal_log.set(price, 0);
            }
        }

        if (volume > 0)
        {
            deal_log.set(deal_price, volume);
            deal_log.state(deal_type_t::buy);
        }
    }

    return make_tuple(deal_log, profits);
}

//----------------------------------------------------------------
//----------------------------------------------------------------

template<typename DealT, typename DealLogT>
tuple< DealLogT, vector<double> >
add_sell_deal(const DealT& deal, const DealLogT& _deal_log)
{
    DealLogT deal_log{_deal_log};
    const auto deal_price = deal.price();
    const auto deal_volume = deal.volume();

    vector<double> profits;

    if ((deal_log.state() == deal_type_t::sell) || (deal_log.state() == deal_type_t::undefined))
    {
        deal_log.set(deal_price,
                     deal_log[deal_price] + deal_volume);
        deal_log.state(deal_type_t::sell);
    }
    else
    {
        auto volume = deal_volume;
        for (double price : deal_log.buys())
        {
            if (volume <= deal_log[price])
            {
                profits.push_back( volume * (deal_price - price) );
                deal_log.set(price,
                             deal_log[price] - volume);
                volume = 0;
                break;
            }
            else
            {
                profits.push_back( deal_log[price] * (deal_price - price) );
                volume -= deal_log[price];
                deal_log.set(price, 0);
            }
        }

        if (volume > 0)
        {
            deal_log.set(deal_price, volume);
            deal_log.state(deal_type_t::sell);
        }
    }

    return make_tuple(deal_log, profits);
}

//----------------------------------------------------------------
//----------------------------------------------------------------

}
}

#endif // ADD_DEAL_HPP
