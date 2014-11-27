#ifndef CG_SYMBOL_T_H
#define CG_SYMBOL_T_H

#include "cg_headers.h"
#include "../symbol_base_t.h"

namespace cgatepp
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class cg_symbol_t : public trading::symbol_base_t
{
public:
    cg_symbol_t(const string& name) :
        symbol_base_t{name}
    {
    }

    void cg_isin_id(int32_t cg_isin_id) { m_cg_isin_id = cg_isin_id; }
    int32_t cg_isin_id() const { return m_cg_isin_id; }

    bool equal(const trading::symbol_base_t& symbol) const override
    {
        const cg_symbol_t& cg_symbol = static_cast<const cg_symbol_t&>(symbol);
        if (this == &cg_symbol)
            return true;
        if (m_cg_isin_id == cg_symbol.cg_isin_id())
            return true;
        else
            return false;
    }

    bool is_ready() const override { return m_is_ready; }
    void ready() { m_is_ready = true; }

private:
    int32_t m_cg_isin_id;
    bool m_is_ready = false;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}

#endif // CG_SYMBOL_T_H
