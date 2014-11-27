#ifndef ROBOT_BASE_T_H
#define ROBOT_BASE_T_H

#include "headers.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class robot_base_t
{
public:
    robot_base_t();
    virtual ~robot_base_t();

    virtual void add_order(const order_base_t& order) = 0;
    virtual void del_order(const order_base_t& order) = 0;
    virtual void move_order(const order_base_t& order, double price) = 0;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}



#endif // ROBOT_BASE_T_H
