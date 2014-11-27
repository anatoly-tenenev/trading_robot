#ifndef BT_HEADERS_H
#define BT_HEADERS_H

//#include <stdint-gcc.h>
//#include <stddef.h>

#include <vector>
#include <map>
#include <queue>
#include <memory>
#include <algorithm>
#include <functional>
#include <tuple>
#include <fstream>
#include <sstream>

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "../headers.h"
#include "../trade_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

using std::map;
using std::vector;
using std::queue;
using std::unique_ptr;
using std::find_if;
using std::function;
using std::string;
using std::ifstream;
using std::stringstream;

using std::tuple;
using std::make_tuple;
using std::tie;

using boost::posix_time::ptime;
using boost::gregorian::date;
using boost::posix_time::time_duration;
using boost::posix_time::hours;
using boost::posix_time::minutes;
using boost::posix_time::seconds;
using boost::posix_time::millisec;

using boost::date_time::max_date_time;
using boost::date_time::min_date_time;

using trading::order_dir_t;
using trading::order_type_t;
using trading::deal_type_t;

using trading::market_event_t;
using trading::orderbook_event_t;
using trading::tick_event_t;
using trading::event_type_t;

using trading::trade_t;
using trading::tradelog_t;

using trading::vol_t;

using trading::dir_str;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class bt_deal_log_t;
class bt_deal_t;
class bt_market_t;
class bt_marketdata_t;
class bt_order_t;
class bt_orderbook_t;
class bt_orderlog_t;
class bt_robot_t;
class bt_symbol_t;
class bt_task_t;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_HEADERS_H
