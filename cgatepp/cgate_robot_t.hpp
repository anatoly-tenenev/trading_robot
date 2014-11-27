#pragma once

#include "cgate_t.h"
#include "cg_connection_t.h"
#include "futinfo/futinfo_stream_t.h"
#include "cg_listener_repl_t.h"
#include "cg_listener_mqreply_t.h"
#include "cg_publisher_base_t.h"
#include "commands/fut_add_order_t.h"
#include "cg_robot_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cgate_robot_t
{
public:
    cgate_robot_t()
    {

    }

    static void main()
    {
        cgate_t cgate{"ini=robot.ini;key=11111111"};
        cg_connection_t conn{"p2tcp://127.0.0.1:4001;app_name=test_repl"};
        cgate.add_connection(&conn);

        futinfo_stream_t futinfo;
        cg_listener_repl_t lsn{futinfo, conn};

        cg_publisher_base_t fut_add_order_pub{"fut_add_order", conn};
        fut_add_order_t fut_add_order{fut_add_order_pub};
        cg_listener_mqreply_t fut_add_order_lsn{fut_add_order, fut_add_order_pub, conn};

        cg_robot_t robot;
        cgate.add_robot(&robot);
        cgate.run();
    }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
