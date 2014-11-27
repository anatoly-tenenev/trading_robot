#ifndef MARKET_BASE_T_H
#define MARKET_BASE_T_H

#include "headers.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class market_base_t
{
public:
    market_base_t();
    virtual ~market_base_t();

    void add_strategy(strategy_base_t* strategy);
    virtual robot_base_t& robot() = 0;
    virtual void run() = 0;

    virtual ptime datetime() const = 0;

protected:
    vector<strategy_base_t*> m_strategies;

    virtual void init_strategy(strategy_base_t* strategy) = 0;
    void process_strategies();
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // MARKET_BASE_T_H
