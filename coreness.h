#ifndef _CORENESS_H
# define _CORENESS_H
#include "utils.h"
class Coreness {
    friend ostream & operator<<(ostream&, const Coreness&);
public:
    Coreness(const graph &G);
    const nm_t& get() const;
private:
    const graph& g;
    nm_t cn;
    em_t ce;
};

#endif
