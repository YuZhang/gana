#ifndef _REWIRE_H
# define _REWIRE_H
#include "utils.h"
class REWIRE {
public:	
    REWIRE(graph &, int, int, int);
private:
    graph & G;
    int di;
    vector<node> node_v;
    bool swap();
    edge pick_edge(node);
    int edge_exist(node, node);
};
#endif
