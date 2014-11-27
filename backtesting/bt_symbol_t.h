#ifndef BT_SYMBOL_T_H
#define BT_SYMBOL_T_H

#include "bt_headers.h"
#include "../symbol_base_t.h"

namespace backtesting
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class bt_symbol_t : public trading::symbol_base_t
{
public:
    bt_symbol_t(const string& name);

    bool equal(const trading::symbol_base_t& symbol) const override
    {
        const bt_symbol_t& bt_symbol = static_cast<const bt_symbol_t&>(symbol);
        if (this == &bt_symbol)
            return true;
        return m_name == bt_symbol.m_name;
    }

    bool is_ready() const override { return true; }

private:

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // BT_SYMBOL_T_H
