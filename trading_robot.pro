QMAKE_CXXFLAGS += -std=c++11 -DBOOST_LOG_DYN_LINK

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cgatepp/cg_listener_mqreply_t.cpp \
    cgatepp/cg_listener_ordbook_t.cpp \
    cgatepp/cg_listener_repl_t.cpp \
    cgatepp/cg_listener_t.cpp \
    cgatepp/cg_member_base_t.cpp \
    cgatepp/cg_message_base_t.cpp \
    cgatepp/cg_message_mqreply_t.cpp \
    cgatepp/cg_message_t.cpp \
    cgatepp/cg_publisher_base_t.cpp \
    cgatepp/cg_stream_base_t.cpp \
    cgatepp/cg_stream_mqreply_t.cpp \
    cgatepp/cg_stream_t.cpp \
    cgatepp/cg_table_t.cpp \
    cgatepp/cgate_t.cpp \
    cgatepp/cg_connection_t.cpp \
    cgatepp/cg_field_base_t.cpp \
    cgatepp/cg_field_t.cpp \
    cgatepp/cg_listener_base_t.cpp \
    cgatepp/msg_members/cg_index_t.cpp \
    cgatepp/msg_members/cg_local_time_field_t.cpp \
    cgatepp/msg_members/cg_msgid_t.cpp \
    cgatepp/msg_members/cg_replies_t.cpp \
    cgatepp/msg_members/cg_reply_t.cpp \
    cgatepp/msg_members/cg_request_t.cpp \
    cgatepp/futinfo/futinfo_stream_t.cpp \
    cgatepp/futinfo/session_msg_t.cpp \
    cgatepp/commands/fut_add_order_reply_t.cpp \
    cgatepp/commands/fut_add_order_request_t.cpp \
    cgatepp/commands/fut_add_order_t.cpp \
    cgatepp/cg_utils.cpp \
    order_base_t.cpp \
    orderbook_base_t.cpp \
    market_base_t.cpp \
    strategy_base_t.cpp \
    robot_base_t.cpp \
    cgatepp/cg_robot_t.cpp \
    orderlog_base_t.cpp \
    backtesting/bt_market_t.cpp \
    backtesting/bt_robot_t.cpp \
    symbol_base_t.cpp \
    marketdata_base_t.cpp \
    cgatepp/cg_marketdata_t.cpp \
    cgatepp/cg_symbol_t.cpp \
    cgatepp/cg_orderlog_t.cpp \
    cgatepp/cg_order_t.cpp \
    cgatepp/cg_orderbook_t.cpp \
    deal_log_base_t.cpp \
    cgatepp/cg_deal_log_t.cpp \
    backtesting/bt_orderlog_t.cpp \
    backtesting/bt_marketdata_t.cpp \
    backtesting/bt_orderbook_t.cpp \
    backtesting/bt_deal_log_t.cpp \
    backtesting/bt_order_t.cpp \
    backtesting/bt_symbol_t.cpp \
    deal_base_t.cpp \
    backtesting/bt_deal_t.cpp \
    backtesting/bt_task_t.cpp \
    backtesting/functions/add_order.cpp \
    backtesting/functions/handle_tick_event.cpp \
    order_array_t.cpp \
    utils.cpp \
    strategies/scalper_strategy.cpp \
    trade_t.cpp \
    strategies/carcass_strategy_t.cpp \
    order_t.cpp \
    strategies/scalper_order_t.cpp \
    strategies/scalper_trade_t.cpp \
    global.cpp \
    strategies/scalper_strategy_1.cpp \
    market_event_t.cpp \
    strategies/event_writer_t.cpp \
    strategies/scalper_strategy_2.cpp \
    cgatepp/cg_global.cpp \
    cgatepp/futtrade/futtrade_stream_t.cpp \
    cgatepp/futinfo/fut_instruments_msg_t.cpp \
    cgatepp/futinfo/fut_sess_contents_msg_t.cpp \
    cgatepp/cg_market_t.cpp \
    cgatepp/ordlog/orders_log_msg_t.cpp \
    cgatepp/ordlog/ordlog_stream_t.cpp \
    cgatepp/ordbook/ordbook_stream_t.cpp \
    cgatepp/ordbook/orders_msg_t.cpp \
    cgatepp/ordbook/ordbook_info_msg_t.cpp \
    strategies/scalper_strategy_3.cpp \
    strategies/scalper_strategy_4.cpp \
    strategies/logger_strategy.cpp \
    strategies/scalper_strategy_5.cpp

HEADERS += \
    cgatepp/cg_listener_mqreply_t.h \
    cgatepp/cg_listener_ordbook_t.h \
    cgatepp/cg_listener_repl_t.h \
    cgatepp/cg_listener_t.h \
    cgatepp/cg_member_base_t.h \
    cgatepp/cg_message_base_t.h \
    cgatepp/cg_message_mqreply_t.h \
    cgatepp/cg_message_t.h \
    cgatepp/cg_publisher_base_t.h \
    cgatepp/cg_stream_base_t.h \
    cgatepp/cg_stream_mqreply_t.h \
    cgatepp/cg_stream_t.h \
    cgatepp/cg_table_t.h \
    cgatepp/cgate_robot_t.hpp \
    cgatepp/cgate_t.h \
    cgatepp/cg_connection_t.h \
    cgatepp/cg_field_base_t.h \
    cgatepp/cg_field_t.h \
    cgatepp/cg_listener_base_t.h \
    cgatepp/msg_members/cg_index_t.h \
    cgatepp/msg_members/cg_local_time_field_t.h \
    cgatepp/msg_members/cg_msgid_t.h \
    cgatepp/msg_members/cg_replies_t.h \
    cgatepp/msg_members/cg_reply_t.h \
    cgatepp/msg_members/cg_request_t.h \
    cgatepp/futinfo/futinfo_stream_t.h \
    cgatepp/futinfo/session_msg_t.h \
    cgatepp/commands/fut_add_order_reply_t.h \
    cgatepp/commands/fut_add_order_request_t.h \
    cgatepp/commands/fut_add_order_t.h \
    cgatepp/cg_utils.h \
    cgatepp/cg_headers.h \
    order_base_t.h \
    orderbook_base_t.h \
    headers.h \
    market_base_t.h \
    strategy_base_t.h \
    robot_base_t.h \
    cgatepp/cg_robot_t.h \
    orderlog_base_t.h \
    backtesting/bt_market_t.h \
    backtesting/bt_robot_t.h \
    backtesting/bt_headers.h \
    symbol_base_t.h \
    marketdata_base_t.h \
    cgatepp/cg_marketdata_t.h \
    cgatepp/cg_symbol_t.h \
    cgatepp/cg_orderlog_t.h \
    cgatepp/cg_order_t.h \
    cgatepp/cg_orderbook_t.h \
    deal_log_base_t.h \
    cgatepp/cg_deal_log_t.h \
    backtesting/bt_orderlog_t.h \
    backtesting/bt_marketdata_t.h \
    backtesting/bt_orderbook_t.h \
    backtesting/bt_deal_log_t.h \
    backtesting/bt_order_t.h \
    backtesting/bt_symbol_t.h \
    deal_base_t.h \
    backtesting/bt_deal_t.h \
    backtesting/bt_task_t.h \
    backtesting/functions/functions.h \
    orderlog_t.hpp \
    order_array_t.h \
    utils.h \
    types.h \
    functions/add_deal.hpp \
    strategies/scalper_strategy.h \
    trade_t.h \
    program.hpp \
    strategies/carcass_strategy_t.h \
    order_t.h \
    strategies/scalper_order_t.h \
    strategies/scalper_headers.h \
    strategies/scalper_trade_t.h \
    strategies/scalper_strategy_1.h \
    events/orderbook_event_t.hpp \
    events/tick_event_t.hpp \
    market_event_t.h \
    strategies/event_writer_t.h \
    strategies/scalper_strategy_2.h \
    cgatepp/futtrade/futtrade_stream_t.h \
    cgatepp/futinfo/fut_instruments_msg_t.h \
    cgatepp/futinfo/fut_sess_contents_msg_t.h \
    cgatepp/cg_market_t.h \
    cgatepp/ordlog/orders_log_msg_t.h \
    cgatepp/ordlog/ordlog_stream_t.h \
    cgatepp/ordbook/ordbook_stream_t.h \
    cgatepp/ordbook/orders_msg_t.h \
    cgatepp/ordbook/ordbook_info_msg_t.h \
    strategies/scalper_strategy_3.h \
    strategies/scalper_strategy_4.h \
    strategies/logger_strategy.h \
    strategies/scalper_strategy_5.h


LIBS += -lcgate
LIBS += -lboost_date_time
LIBS += -lpthread
LIBS += -lboost_log
LIBS += -lboost_log_setup
LIBS += -lboost_chrono
LIBS += -lboost_filesystem
LIBS += -lboost_system
LIBS += -lboost_thread

system(cp $$PWD/robot.ini $$OUT_PWD/robot.ini)
