#include "coreness.h"

Coreness::Coreness(const graph& G): g(G), cn(G,0), ce(G, 0)
{
    graph G_copy(G);
    list < node > lnode = G_copy.all_nodes();
    vector < int > nc, ec;
    for (int K = 1; !lnode.empty(); ++K) {
        nc.push_back(0);
        ec.push_back(0);
        while (!lnode.empty()) {
            list < node >::iterator it = lnode.begin();
            list < node >::iterator end = lnode.end();
            int clear = 1;
            while (it != end) {
                if (it->degree() > K) {
                    ++it;
                    continue;
                }
                cn[*it] = K - 1;
		nc[K-1]++;
		node::adj_edges_iterator eit, eend;
		eend = it->adj_edges_end();
		for (eit=it->adj_edges_begin(); eit!=eend; ++eit) {
		    ce[*eit] = K - 1;
		    ec[K-1]++;
		}
                G_copy.del_node(*it);
                it = lnode.erase(it);
                clear = 0;
            }
            if (clear) {
//	cout << "Core " << K-1 << " " << nc[K-1] << " " << ec[K-1] << endl;
                break;
            }
        }
    }
}
const nm_t& Coreness::get() const
{
    return cn;
}
ostream& operator<<(ostream& os, const Coreness& cn)
{
    Dstat ds(cn.g, cn.cn);
    os << "Average coreness: " << ds.mean('y') << endl;
    os << "Stdv of coreness: " << sqrt(ds.variance('y')) << endl;
    os << "Node coreness distribution:" << endl;
    ds.freq('y');
    ds.print();
    os << "#" << endl;
    os << "Coreness distribution ";
    ds.linefit(2,13,'z', true);
    /*os << "Node Coreness:" << endl;
    print_node_map(cn.g, cn.cn);
    os << "#" << endl;*/
    /*os << "Edge coreness distribution:" << endl;
    Dstat de(cn.g, cn.ce);
    de.freq('y');
    de.print();
    os << "#" << endl;*/
    //os << "Edge Coreness:" << endl;
    //print_edge_map(cn.g, cn.ce);
    //os << "#" << endl;

    return os;
}
