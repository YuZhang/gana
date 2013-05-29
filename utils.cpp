#include "utils.h"

void print_node_map(const graph & G, const nm_t& nm)
{
    graph::node_iterator it, end;
    end = G.nodes_end();
    for (it = G.nodes_begin(); it != end; ++it) {
        cout <<it->id()<<" "<<nm[*it]<< endl;
    }
}

void print_edge_map(const graph & G, const em_t& em)
{
    graph::edge_iterator it, end;
    end = G.edges_end();
    for (it = G.edges_begin(); it != end; ++it) {
        cout <<it->source().id()<<" "<<it->target().id()<<" "<<em[*it]<< endl;
    }
}


