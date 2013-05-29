#include "clustering.h"

template < class NIT > int nr_links_with_node(NIT, NIT, node);
template < class NIT > int nr_links_among_nodes(NIT, NIT);
static bool compare_by_degree(node, node);

template < class NIT > int nr_links_with_node(NIT begin, NIT end, node n)
{
    int nr_links = 0;
    node::adj_nodes_iterator adjb, adje, result;

    adjb = n.adj_nodes_begin();
    adje = n.adj_nodes_end();
    for (; begin != end; ++begin) {
        result = find(adjb, adje, *begin);
        if (result != adje) {
            nr_links++;
        }
    }
    return nr_links;
}

template < class NIT > int nr_links_among_nodes(NIT begin, NIT end)
{
    int nr_links = 0;

    for (; begin != end; ++begin) {
        nr_links += nr_links_with_node(begin, end, *begin);
    }
    return nr_links;
}

/* c1 is the average of local clustering coefficient (CC) of node;
 * c2 is the transitivity of adjacent matrix, alternative CC.
 */
Cluster::Cluster(graph& G):g(G), nc(G), cc(0), tt(0)
{
    double nr_2d_node = 0;
    double nr_adj_edge_pair = 0;
    graph::node_iterator it, end;
    end = g.nodes_end();
    for (it = g.nodes_begin(); it != end; ++it) {
        node n = *it;
        int d = n.degree();
        if (d < 2) {
            nc[n] = (-1);
            continue;
        }
        int t = nr_links_among_nodes(n.adj_nodes_begin(), n.adj_nodes_end());
        nc[n] = t * 2.0 / (d * (d - 1));
        cc += nc[n];
        tt += t;
        nr_2d_node++;
        nr_adj_edge_pair += d * (d - 1) / 2.0;
    }
    cc /= g.number_of_nodes();
    tt /= nr_adj_edge_pair;
}
ostream& operator<<(ostream& os, const Cluster& ct)
{
    cout << "Average Clustering: " << ct.cc << endl;
    cout << "Transitivity: " << ct.tt << endl;
    os << "Node clustering:" << endl;
    print_node_map(ct.g, ct.nc);
    os <<"#" << endl;
    return os;
}

static bool compare_by_degree(node a, node b)
{
    return (a.degree() > b.degree());
}

Richclub::Richclub(graph & G): g(G)
{
    double nrl = 0;
    int nrn = G.number_of_nodes();
    int max_clique = 1;
    int max_clique2 = 1;
    vector < node > vnode(nrn);
    node_map < int >rank(G);
    copy(G.nodes_begin(), G.nodes_end(), vnode.begin());
    stable_sort(vnode.begin(), vnode.end(), compare_by_degree);
    for (int i = 0; i != nrn; ++i) {
        rank[vnode[i]] = i + 1;
//cout << "+" << vnode[i].degree() << endl;
    }
    int last_d = vnode[0].degree();
    for (int j = 2; j != nrn + 1; ++j) {
        node::adj_nodes_iterator adjb, adje;
        adje = vnode[j - 1].adj_nodes_end();
        for (adjb = vnode[j - 1].adj_nodes_begin(); adjb != adje; ++adjb) {
            if (rank[*adjb] < rank[vnode[j - 1]]) {
	//cout << "!" << rank[*adjb] << " " << rank[vnode[j - 1]] << endl;
	//cout << "!" << *adjb << " " << vnode[j - 1] << endl;
                nrl += 1;
            }
        }
        rr[j] = nrl * 2 / ((double) j * (j - 1));
	if (vnode[j-1].degree() != last_d && j!=2) {
            dr[last_d] = rr[j-1];
    	    if (1 - dr[last_d] < 1e-10) {
                max_clique2 = last_d;
            }
            last_d = vnode[j-1].degree();
        }
        if (1 - rr[j] < 1e-10) {
            max_clique = j;
        }
    }
    cout << "Max clique: " << max_clique << endl;
    double t = rr[int(0.01*nrn)];
    //double t = rr[3];
    cout << "Rich-club connectivity: " << t << endl;

}

ostream& operator << (ostream& os, const Richclub& rc)
{
    Dstat ds1(rc.rr);
    os << "RR data:" << endl;
    ds1.print();
    os << "#" << endl;
    os << "Rank rich-club connectivity " ;
    ds1.linefit(100, 1000000, 'z', false);
    Dstat ds2(rc.dr);
    os << "Degree rich-club connectivity:" << endl;
    ds2.print();
    os << "#" << endl;
    os << "Degree rich-club connectivity " ;
    ds2.linefit(6, 40, 'z', false);
    return os;
}
