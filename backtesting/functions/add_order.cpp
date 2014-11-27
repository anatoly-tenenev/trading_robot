#include "functions.h"
#include "../bt_order_t.h"
#include "../bt_orderbook_t.h"
#include "../bt_deal_t.h"
#include "../bt_symbol_t.h"

namespace backtesting {
namespace functions {

//----------------------------------------------------------------
//----------------------------------------------------------------

tuple< vector<bt_order_t>, bt_orderbook_t, vector<bt_deal_t> >
add_bid_order(const bt_order_t& order, const bt_orderbook_t& _orderbook);

tuple< vector<bt_order_t>, bt_orderbook_t, vector<bt_deal_t> >
add_ask_order(const bt_order_t& order, const bt_orderbook_t& _orderbook);

//----------------------------------------------------------------
//----------------------------------------------------------------

tuple< vector<bt_order_t>, bt_orderbook_t, vector<bt_deal_t> >
add_order(const bt_order_t& order, const bt_orderbook_t& orderbook)
{
    switch ( order.dir() )
    {
    case order_dir_t::bid:
        return add_bid_order(order, orderbook);
    case order_dir_t::ask:
        return add_ask_order(order, orderbook);
    default:
        return make_tuple(vector<bt_order_t>(), orderbook, vector<bt_deal_t>());
    }
}

//----------------------------------------------------------------
//----------------------------------------------------------------

tuple< vector<bt_order_t>, bt_orderbook_t, vector<bt_deal_t> >
add_bid_order(const bt_order_t& _order, const bt_orderbook_t& _orderbook)
{
    const bt_symbol_t& symbol =
            static_cast<const bt_symbol_t&>( _order.symbol() );
    bt_order_t order{_order};
    vector<bt_order_t> orders;
    bt_orderbook_t orderbook{_orderbook};
    vector<bt_deal_t> deals;
    const auto step = order.symbol().step();

    if (order.price() >= orderbook.best_ask())
    {
        auto price = orderbook.best_ask();

        while ((price <= order.price()) && (order.volume() > 0))
        {
            if (orderbook.asks[price] >= order.volume())
            {
                deals.push_back( bt_deal_t(symbol, deal_type_t::buy, price, order.volume(), order.id()) );
                orderbook.asks.set(price,
                                   orderbook.asks[price] - order.volume());
                order.volume(0);
                break;
            }
            else
            {
                if (orderbook.asks[price] > 0)
                {
                    deals.push_back( bt_deal_t(symbol, deal_type_t::buy, price, orderbook.asks[price], order.id()) );
                    order.volume( order.volume() - orderbook.asks[price] );
                    orderbook.asks.set(price, 0);
                }

                if (price == order.price())
                {
                    orders.push_back( order );
                    break;
                }

                price += step;
            }
        }
    }
    else
    {
        bt_order_t new_order{order};
        new_order.orders_before( orderbook.bids[order.price()] );
        orders.push_back(new_order);
    }

    return make_tuple(orders, orderbook, deals);
}

//----------------------------------------------------------------
//----------------------------------------------------------------

tuple< vector<bt_order_t>, bt_orderbook_t, vector<bt_deal_t> >
add_ask_order(const bt_order_t& _order, const bt_orderbook_t& _orderbook)
{
    const bt_symbol_t& symbol =
            static_cast<const bt_symbol_t&>( _order.symbol() );
    bt_order_t order{_order};
    vector<bt_order_t> orders;
    bt_orderbook_t orderbook{_orderbook};
    vector<bt_deal_t> deals;
    const auto step = order.symbol().step();

    if (order.price() <= orderbook.best_bid())
    {
        auto price = orderbook.best_bid();

        while ((price >= order.price()) && (order.volume() > 0))
        {
            if (orderbook.bids[price] >= order.volume())
            {
                deals.push_back( bt_deal_t(symbol, deal_type_t::sell, price, order.volume(), order.id()) );
                orderbook.bids.set(price,
                                   orderbook.bids[price] - order.volume());
                order.volume(0);
                break;
            }
            else
            {
                if (orderbook.bids[price] > 0)
                {
                    deals.push_back( bt_deal_t(symbol, deal_type_t::sell, price, orderbook.bids[price], order.id()) );
                    order.volume( order.volume() - orderbook.bids[price] );
                    orderbook.bids.set(price, 0);
                }

                if (price == order.price())
                {
                    orders.push_back( order );
                    break;
                }

                price -= step;
            }
        }
    }
    else
    {
        bt_order_t new_order{order};
        new_order.orders_before( orderbook.asks[order.price()] );
        orders.push_back(new_order);
    }

    return make_tuple(orders, orderbook, deals);
}

//----------------------------------------------------------------
//----------------------------------------------------------------

}
}
