#ifndef _MATCH_H
# define _MATCH_H
#include "utils.h"
class MATCH {
public:	
    MATCH (graph &, int, int);
private:
    graph & G;
    int D;
    edge pick_edge(node);
    int edge_exist(node, node);
};
#endif
