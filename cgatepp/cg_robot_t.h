#ifndef CG_ROBOT_T_H
#define CG_ROBOT_T_H

#include "cg_headers.h"
#include "../robot_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_robot_t : public trading::robot_base_t
{
public:
    cg_robot_t(cg_market_t& market);

    void add_order(const trading::order_base_t& order) override;
    void del_order(const trading::order_base_t& order) override;
    void move_order(const trading::order_base_t& order, double price) override;

private:
    cg_market_t& m_market;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_ROBOT_T_H
