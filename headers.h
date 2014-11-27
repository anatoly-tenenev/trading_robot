#ifndef HEADERS_H
#define HEADERS_H

//#include <stddef.h>

#include <vector>
#include <list>
#include <map>
#include <memory>
#include <functional>
#include <string>
#include <sstream>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <cmath>
//#include <iostream>

#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

using std::vector;
using std::list;
using std::map;
using std::unique_ptr;
using std::string;
using std::stringstream;
using std::ifstream;

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

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class deal_log_base_t;
class deal_base_t;
class market_base_t;
class marketdata_base_t;
class order_array_t;
class order_base_t;
class order_t;
class orderbook_base_t;
class orderlog_base_t;
class robot_base_t;
class strategy_base_t;
class symbol_base_t;
struct trade_t;
class tradelog_t;
struct quote_t;

class market_event_t;
class orderbook_event_t;
class tick_event_t;
enum class event_type_t { orderbook, tick };

template <typename OrderT> class orderlog_t;

//typedef unique_ptr<order_base_t> order_ptr_t;

typedef int vol_t;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

enum class order_dir_t { bid, ask };
enum class order_type_t { limit, market };
enum class deal_type_t { buy, sell, undefined };

//-------------------------------------------------------------------
//-------------------------------------------------------------------

extern const char* dir_str[];
extern const char* time_format;
extern const char* datetime_format;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // HEADERS_H
