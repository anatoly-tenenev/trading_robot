#include "cg_market_t.h"
#include "cg_symbol_t.h"
#include "cg_robot_t.h"

#include "../strategy_base_t.h"

#include <boost/algorithm/string/trim.hpp>

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

cg_market_t::cg_market_t()
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

trading::robot_base_t& cg_market_t::robot()
{
    return m_robot;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_market_t::run()
{
    while (true)
    {
        switch (m_conn.get_state())
        {
        case CG_STATE_ERROR:
            m_conn.close();
            break;

        case CG_STATE_CLOSED:
            m_conn.open();
            break;

        case CG_STATE_ACTIVE:
            m_conn.process();
            break;

        default:
            break;
        }

        for (auto& symbol : m_symbols_to_init)
            if (!symbol->is_ready())
                try_init_symbol(*symbol);

        while (!m_tasks.empty())
        {
            auto task = m_tasks.front();
            m_tasks.pop();
            task();
        }

        process_strategies();
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_market_t::try_init_symbol(cg_symbol_t& symbol)
{
    using boost::algorithm::trim;
    string symbol_name = symbol.name();
    trim(symbol_name);

    auto& table = futinfo.fut_sess_contents_msg.table;
    for (auto it = table.begin(); it != table.end(); it++)
    {
        auto& row = it->second;
        string isin = row.isin;
        trim(isin);
        std::cout << isin << std::endl;
        isin.resize( std::char_traits<char>::length(isin.c_str()) );
        if (symbol_name == isin)
        {
            std::cout << "init" << std::endl;
            symbol.cg_isin_id(row.isin_id);
            symbol.step(row.step_price);
            symbol.ready();
            m_tasks.push([this]()
            {
                m_symbols_to_init.erase(std::remove_if(m_symbols_to_init.begin(),
                                                       m_symbols_to_init.end(),
                                                       [](const cg_symbol_t* symbol)
                                                       { return symbol->is_ready(); }),
                                        m_symbols_to_init.end());

                // если все символы удалены, значит все ордербуки
                // проинициализированы и в vector'ы можно прекратить
                // добавлять новые записи
                if (m_symbols_to_init.empty())
                {
                    ordlog.orders_log_msg.should_update = false;
                    ordbook.orders_msg.should_update = false;
                }
            });
            return;
        }
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cg_market_t::init_strategy(trading::strategy_base_t* strategy)
{
    m_symbols_to_init.push_back(&static_cast<cg_symbol_t&>(strategy->symbol()));
    m_orderlogs.push_back(&static_cast<cg_orderlog_t&>(strategy->orderlog()));
    m_deal_logs.push_back(&static_cast<cg_deal_log_t&>(strategy->deal_log()));
    m_orderbooks.push_back(&static_cast<cg_orderbook_t&>(strategy->orderbook()));

    ordlog.orders_log_msg.symbols.push_back(&static_cast<cg_symbol_t&>(strategy->symbol()));
    ordlog.orders_log_msg.orderbooks.push_back(&static_cast<cg_orderbook_t&>(strategy->orderbook()));
    ordbook.orders_msg.symbols.push_back(&static_cast<cg_symbol_t&>(strategy->symbol()));
    ordbook.orders_msg.orderbooks.push_back(&static_cast<cg_orderbook_t&>(strategy->orderbook()));
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}
