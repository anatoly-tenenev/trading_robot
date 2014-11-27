#ifndef SYMBOL_BASE_T_H
#define SYMBOL_BASE_T_H

#include "headers.h"

namespace trading
{

//-------------------------------------------------------------------
//-------------------------------------------------------------------

class symbol_base_t
{
public:
    symbol_base_t(const string& name);
    virtual ~symbol_base_t();

    virtual bool equal(const symbol_base_t& symbol) const = 0;

    string name() const { return m_name; }

    void step(double step) { m_step = step; }
    double step() const { return m_step; }

    virtual bool is_ready() const = 0;

protected:
    string m_name;
    double m_step;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

}


#endif // SYMBOL_BASE_T_H
