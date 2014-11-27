#ifndef BT_TASK_T_H
#define BT_TASK_T_H

#include "bt_headers.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class bt_task_t
{
public:
    ptime datetime;
    function<void()> execute;

    bt_task_t(const ptime& _datetime,
              const function<void()>& _func) :
        datetime(_datetime),
        execute(_func)
    {

    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_TASK_T_H
