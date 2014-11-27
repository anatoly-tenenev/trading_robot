#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "../bt_headers.h"
#include "../../functions/add_deal.hpp"

namespace backtesting {
namespace functions {

using namespace backtesting;

using trading::functions::add_deal;

tuple<vector<bt_order_t>, bt_orderbook_t, vector<bt_deal_t>>
add_order(const bt_order_t& order, const bt_orderbook_t& orderbook);

tuple<bt_orderlog_t, vector<bt_deal_t>>
handle_tick_event(const bt_deal_t& tick, const bt_orderlog_t& orderlog);

}
}

#endif // FUNCTIONS_H
