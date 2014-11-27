#include "../functions/functions.h"
#include "../bt_deal_t.h"
#include "../bt_orderlog_t.h"
#include "../bt_order_t.h"

namespace backtesting {
namespace functions {

//----------------------------------------------------------------
//----------------------------------------------------------------

tuple< bt_orderlog_t, vector<bt_deal_t> >
handle_buy_tick(const bt_deal_t& _tick, const bt_orderlog_t& _orderlog);

tuple< bt_orderlog_t, vector<bt_deal_t> >
handle_sell_tick(const bt_deal_t& _tick, const bt_orderlog_t& _orderlog);

//----------------------------------------------------------------
//----------------------------------------------------------------

tuple<bt_orderlog_t, vector<bt_deal_t>>
handle_tick_event(const bt_deal_t& tick, const bt_orderlog_t& orderlog)
{
    switch (tick.type())
    {
    case deal_type_t::buy:
        return handle_buy_tick(tick, orderlog);
    case deal_type_t::sell:
        return handle_sell_tick(tick, orderlog);
    default:
        return make_tuple(orderlog, vector<bt_deal_t>());
    }
}

//----------------------------------------------------------------
//----------------------------------------------------------------

tuple< bt_orderlog_t, vector<bt_deal_t> >
handle_buy_tick(const bt_deal_t& _tick, const bt_orderlog_t& _orderlog)
{
    const bt_symbol_t& symbol =
            static_cast<const bt_symbol_t&>( _tick.symbol() );
    vector<bt_deal_t> deals;
    bt_orderlog_t orderlog{_orderlog};
    bt_deal_t tick{_tick};

    auto asks = orderlog.asks();

    for (size_t i = 0; i < asks.size(); i++)
    {
        bt_order_t& order = static_cast<bt_order_t&>(asks[i]);
        if (tick.price() <= order.price())
            break;
        order.orders_before(0);
    }

    for (size_t i = 0; i < asks.size(); i++)
    {
        bt_order_t& order = static_cast<bt_order_t&>(asks[i]);

        if (order.volume() <= 0) continue;

        vol_t volume = 0;

        if ((tick.price() < order.price()) || (tick.volume() == 0))
            break;
        else if (tick.price() == order.price())
        {
            if (order.orders_before() >= tick.volume())
            {
                // т.к. сначала идут заявки, которые ближе всего к исполнению,
                // то все заявки по данной цене сдвигаем вниз
                order.orders_before( order.orders_before() - tick.volume() );
                continue;
            }
            else // order.ordersBefore < deal.volume
            {
                // смотрим сколько заявок сможет зацепить сделка
                // объем сделки уменьшается на величину объема исполненной заявки
                vol_t deal_volume = tick.volume() - order.orders_before();
                if (deal_volume >= order.volume())
                {
                    volume = order.volume();
                    tick.volume( tick.volume() - volume );
                    order.volume(0);
                }
                else // deal_volume < order.volume
                {
                    volume = deal_volume;
                    tick.volume( tick.volume() - volume );
                    order.volume( order.volume() - volume );
                }
                order.orders_before(0);
            }
        }
        else if (tick.price() > order.price())
        {
            if (tick.volume() >= order.volume())
            {
                volume = order.volume();
                tick.volume( tick.volume() - volume );
                order.volume(0);
            }
            else // if (deal.volume < order.volume)
            {
                volume = tick.volume();
                tick.volume(0);
                order.volume( order.volume() - volume );
            }
        }

        deals.push_back( bt_deal_t(symbol, deal_type_t::sell, order.price(), volume, order.id(), tick.datetime()) );
    }

    orderlog.clean_up();

    return make_tuple(orderlog, deals);
}

//----------------------------------------------------------------
//----------------------------------------------------------------

tuple< bt_orderlog_t, vector<bt_deal_t> >
handle_sell_tick(const bt_deal_t& _tick, const bt_orderlog_t& _orderlog)
{
    const bt_symbol_t& symbol =
            static_cast<const bt_symbol_t&>( _tick.symbol() );
    vector<bt_deal_t> deals;
    bt_orderlog_t orderlog{_orderlog};
    bt_deal_t tick{_tick};

    auto bids = orderlog.bids();

    for (size_t i = 0; i < bids.size(); i++)
    {
        bt_order_t& order = static_cast<bt_order_t&>(bids[i]);
        if (tick.price() >= order.price())
            break;
        order.orders_before(0);
    }

    for (size_t i = 0; i < bids.size(); i++)
    {
        bt_order_t& order = static_cast<bt_order_t&>(bids[i]);

        if (order.volume() <= 0) continue;

        vol_t volume = 0; // объем вероятной сделки сделки

        if ((tick.price() > order.price()) || (tick.volume() == 0))
            break;
        else if (tick.price() == order.price())
        {
            if (order.orders_before() >= tick.volume())
            {
                // т.к. сначала идут заявки, которые ближе всего к исполнению,
                // то все заявки по данной цене сдвигаем вниз
                order.orders_before( order.orders_before() - tick.volume() );
                continue;
            }
            else // order.ordersBefore < deal.volume
            {
                // смотрим сколько заявок сможет зацепить сделка
                // объем сделки уменьшается на величину объема исполненной заявки
                vol_t deal_volume = tick.volume() - order.orders_before();
                if (deal_volume >= order.volume())
                {
                    volume = order.volume();
                    tick.volume( tick.volume() - volume );
                    order.volume(0);
                }
                else // deal_volume < order.volume
                {
                    volume = deal_volume;
                    tick.volume( tick.volume() - volume );
                    order.volume( order.volume() - volume );
                }
                order.orders_before(0);
            }
        }
        else if (tick.price() < order.price())
        {
            if (tick.volume() >= order.volume())
            {
                volume = order.volume();
                tick.volume( tick.volume() - volume );
                order.volume(0);
            }
            else // if (deal.volume < order.volume)
            {
                volume = tick.volume();
                tick.volume(0);
                order.volume( order.volume() - volume );
            }
        }

        deals.push_back( bt_deal_t(symbol, deal_type_t::buy, order.price(), volume, order.id(), tick.datetime()) );
    }

    orderlog.clean_up();

    return make_tuple(orderlog, deals);
}

//----------------------------------------------------------------
//----------------------------------------------------------------

}
}

