#ifndef __SP_BASED_H
#  define __SP_BASED_H
#include "utils.h"

class Sp_based {
    friend ostream& operator<<(ostream&, const Sp_based&);
public:
    Sp_based(const graph&);
private:
    const graph& og;
    graph g;
    components compo;
    node_map <char> inQ;
    node_map < list<node> > tt;
    node_map < components::component_iterator >on_compo;
    node_map < int > coms;
    node_map < int > n_stub;
    node_map < int > dd;
    node_map< vector< int > > d_stub;
    nm_t avgd;
    nm_t maxd;
    nm_t nbet;
    em_t ebet;
    data_t pairs;
    void id2compo();
    void tree(const node&);
    void shrink();
    void shortest_path();
};

#endif
