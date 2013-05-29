#include "rewire.h"
#include "random.h"
static KW_RNG::RNG _my_random;
static void my_srandom(int x) { _my_random.init(x,!x*13,x*x+1,(x>>16)+(x<<16)); }
static double my_random01() { return _my_random.rand_halfclosed01(); }

REWIRE::REWIRE(graph & g, int s, int r, int d): G(g), di(d)
{
   my_srandom(s);
   int n = r*G.number_of_edges();
   graph::node_iterator it, end = G.nodes_end();
   for(it=G.nodes_begin(); it!=end; ++it) {
       node_v.push_back(*it);
   }
   for(int i=0; i<n; ++i) {
     if (!swap()) {
        break;
     } 
    /* if (i%40000==0) {
	cerr << "\r" << i*100.0/n << "%";
     }*/
   }
   /*cerr << "\r 100%" << endl;*/
}

int REWIRE::edge_exist(node u, node v)
{
    node::adj_nodes_iterator adjb, adje, result;
    //cout << " " << u << " " << v << endl;
    if (u==v) {return 1;}
    if (u.degree()<v.degree()) {
        adjb = u.adj_nodes_begin();
        adje = u.adj_nodes_end();
        result = find(adjb, adje, v);
    } else {
        adjb = v.adj_nodes_begin();
        adje = v.adj_nodes_end();
        result = find(adjb, adje, u);
    }
    if (result != adje) { return 1; }
    return 0;
}
edge REWIRE::pick_edge(node n)
{
    node::adj_edges_iterator it, end=n.adj_edges_end();
    int r = (int) (my_random01()*n.degree());
    int i = 0;
    for(it=n.adj_edges_begin(); it!=end; ++it) {
      if (i==r) {
        return *it;
      }  
      i++;
    }
    cerr << "pick_edge " <<  r  << " " <<  n.degree() << endl;
    exit(1);
}

static bool compare_by_degree(node a, node b)
{
    return (a.degree() > b.degree());
}

bool REWIRE::swap()
{
    int N=G.number_of_nodes();
    node u, v;
    int fail=0;
    vector < node > vn(4);
SWAPS:
    if (fail > 1000) {
       return false;
    }
    do {
        u = node_v[(int) (my_random01()*N)];
        v = node_v[(int) (my_random01()*N)];
    } while(u == v);
    edge e1 = pick_edge(u);
    edge e2 = pick_edge(v);
    node u1 = e1.source()==u?e1.target():e1.source();
    node v1 = e2.source()==v?e2.target():e2.source();
    /*
    if (u1 == u or v1 ==v) {
	cerr << "swap " << u << " " << u1 << " " << v << " " << v1 <<endl;
	exit(1);
    }*/
    if(u1==v1 or u==v1 or v==u1 or edge_exist(u, v1) or edge_exist(v, u1)) {
        fail++;
	goto SWAPS;
    }
    vn[0]=u; vn[1]=u1; vn[2]=v; vn[3]=v1;
    stable_sort(vn.begin(), vn.end(), compare_by_degree);
    switch(di) {
      case 0:
	    G.new_edge(u, v1);
            G.new_edge(v, u1);
            G.del_edge(e1);
            G.del_edge(e2);
	    break;
      case 1:// assortative
	    if(not edge_exist(vn[0], vn[1]) and not edge_exist(vn[2], vn[3])) {
                G.new_edge(vn[0],vn[1]);
                G.new_edge(vn[2],vn[3]);
                G.del_edge(e1);
                G.del_edge(e2);
            }
            break;
      case 2://disassortative
	    if(not edge_exist(vn[0], vn[3]) and not edge_exist(vn[1], vn[2])) {
                G.new_edge(vn[0],vn[3]);
                G.new_edge(vn[1],vn[2]);
                G.del_edge(e1);
                G.del_edge(e2);
            }
	    break;
      case 3:
	    if(not edge_exist(vn[0], vn[3]) and not edge_exist(vn[1], vn[2])) {
                G.new_edge(vn[0],vn[3]);
                G.new_edge(vn[1],vn[2]);
                G.del_edge(e1);
                G.del_edge(e2);
            }
	    break;
    }
   
    return true;
}


