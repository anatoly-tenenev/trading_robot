#ifndef BT_ROBOT_T_H
#define BT_ROBOT_T_H

#include "bt_headers.h"
#include "../robot_base_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class bt_robot_t : public trading::robot_base_t
{
public:
    bt_robot_t(bt_market_t& market);

    void add_order(const trading::order_base_t& order) override;
    void del_order(const trading::order_base_t& order) override;
    void move_order(const trading::order_base_t& order, double price) override;

private:
    bt_market_t& m_market;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_ROBOT_T_H
