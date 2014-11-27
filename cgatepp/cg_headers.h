#ifndef CG_HEADERS_H
#define CG_HEADERS_H

//-------------------------------------------------------------------
//-------------------------------------------------------------------

#include <cgate.h>

#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <queue>
#include <fstream>
#include <iostream>

#include <boost/date_time/posix_time/posix_time_types.hpp>

//-------------------------------------------------------------------
//-------------------------------------------------------------------

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

using std::stringstream;
using std::string;
using std::vector;
using std::map;
using std::function;
using std::queue;

using boost::posix_time::ptime;
using boost::gregorian::date;
using boost::posix_time::time_duration;
using boost::posix_time::hours;
using boost::posix_time::minutes;
using boost::posix_time::seconds;
using boost::posix_time::millisec;
using boost::posix_time::microsec_clock;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_member_base_t;
class cg_field_base_t;
class cg_index_t;
class cg_msgid_t;
class cg_request_t;
class cg_reply_t;
class cg_local_time_field_t;
class cg_replies_t;
class cg_market_t;
class cg_marketdata_t;
class cg_symbol_t;
class cg_orderlog_t;
class cg_order_t;
class cg_orderbook_t;
class cg_deal_log_t;

class cg_listener_base_t;
class cg_listener_t;
class cg_listener_mqreply_t;
class cg_listener_repl_t;
class cg_listener_ordbook_t;
class cg_publisher_base_t;
class cg_message_base_t;
class cg_message_mqreply_t;
class cg_message_t;
class cg_stream_base_t;
class cg_stream_t;
class cg_stream_mqreply_t;
class cg_robot_t;
class cg_connection_t;
class cgate_t;

template<typename T> class cg_field_t;

//-------------------------------------------------------------------
//-------------------------------------------------------------------



//-------------------------------------------------------------------
//-------------------------------------------------------------------

enum class cg_field_types_t { i1, i2, i4, i8, cN, dNM, t, f };

enum class cg_subscr_type_t { p2repl, p2mqreply, p2ordbook };
extern const char* cg_subscr_type_str[];

enum class cg_listener_mode_t { snapshot, online, snapshot_online };
extern const char* cg_lisntener_mode_str[];

//-------------------------------------------------------------------
//-------------------------------------------------------------------

typedef decltype(::cg_message_desc_t::id) msg_id_t;

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

#endif // CG_HEADERS_H
