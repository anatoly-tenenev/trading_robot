#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "backtesting/bt_market_t.h"
#include "backtesting/bt_marketdata_t.h"
#include "backtesting/bt_symbol_t.h"

#include "cgatepp/cg_market_t.h"
#include "cgatepp/cg_marketdata_t.h"
#include "cgatepp/cg_symbol_t.h"

//#include "strategies/scalper_strategy_3.h"
#include "strategies/scalper_strategy_4.h"
//#include "strategies/logger_strategy.h"
//#include "strategies/carcass_strategy_t.h"

#include <iostream>
#include <string>

class Program
{
public:

    void main()
    {
        using namespace std;
        using namespace strategies;

        string rts = "RTS-9.14";

        using namespace backtesting;
        bt_symbol_t symbol{rts};
        symbol.step(10.0);
        bt_marketdata_t marketdata;
        bt_market_t market{"06-18.csv"};

//        using namespace cgatepp;
//        cg_symbol_t symbol{rts};
//        cg_marketdata_t marketdata;
//        cg_market_t market;

        using namespace trading;
        scalper_strategy_4_t strategy{market, symbol, marketdata};
        //logger_strategy_t strategy{market, symbol, marketdata};
        //carcass_strategy_t strategy{market, symbol, marketdata};

        std::cout << "run" << std::endl;

        market.run();

        cout << "result: " << strategy.result() << endl
             << "pos total: " << strategy.tradelog().pos_total() << endl
             << "pos trades: " << strategy.tradelog().pos_trades() << endl
             << "neg total: " << -strategy.tradelog().neg_total() << endl
             << "neg trades: " << strategy.tradelog().neg_trades() << endl;
    }
};

#endif // PROGRAM_HPP
