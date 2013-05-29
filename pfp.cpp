#include "pfp.h"
#include "random.h"
static KW_RNG::RNG _my_random;
static void my_srandom(int x) { _my_random.init(x,!x*13,x*x+1,(x>>16)+(x<<16)); }
static double my_random01() { return _my_random.rand_halfclosed01(); }

PFP::PFP(graph & g, int n, int s): G(g) 
{
   my_srandom(s);
   N = n;
   sN = 10;
   sM = 30;
   D = 0.021;
   P = 0.4;
   pi.push_back(0);
   for (int i=1; i<5000; i++) {
     pi.push_back(std::pow(i, 1.0+D*std::log((double)i)));
   }
   G.clear();
}

int PFP::edge_exist(int u, int v)
{
    node::adj_nodes_iterator adjb, adje, result;

    //cout << " " << u << " " << v << endl;
    if (u==v) {return 1;}
    adjb = node_v[u].adj_nodes_begin();
    adje = node_v[u].adj_nodes_end();
    result = find(adjb, adje, node_v[v]);
    if (result != adje) { return 1; }
    G.new_edge(node_v[u], node_v[v]);
    return 0;
}

void PFP::gen_rn(int N, int M)
{
    int u, v;

    for (int i = 0; i < N; i++) {
        node tmp_node = G.new_node();
        node_v.push_back(tmp_node);
	deg.push_back(0);
    }
    for (int i = 0; i < M; i++) {
        do {
            u = (int) (my_random01()*N);
            v = (int) (my_random01()*N);
	} while(edge_exist(u, v));
	deg[u]++; deg[v]++;

    }
    /*
    cout << "Graph is connected ? " << (G.is_connected() ==
               true ? "true" : "false")
         << endl;*/
}

double PFP::cal_Psum2()
{
    double a = 0;
    graph::node_iterator it, end;
    end = G.nodes_end();
    for (it = G.nodes_begin(); it != end; ++it) {
        int d = it->degree();
        a += pi[d]; 
    }
    return a;
}

double PFP::cal_Psum()
{
    double a = 0;
    int it, end;
    end = node_v.size();
    for (it = 0; it < end; ++it) { a += pi[deg[it]]; }
    return a;
}

int PFP::pick()
{
    double p2 = 0;
    double q = my_random01()*Psum;
    int it, end;
    end = node_v.size();
    for (it = 0; it < end; ++it) {
        p2 += pi[deg[it]]; 
	if (q < p2) {return it;}
    }
    Psum=cal_Psum();
    return end-1;
/*    cout.precision(12);
    cout << "PICK " << q << " " << p2 << " " 
         << Psum << " " << cal_Psum() <<endl;*/
}

void PFP::gen_pfp()
{
    int u, v, w, x, y;
    node new_node;
    G.make_undirected();
    gen_rn(sN, sM);
    Psum=cal_Psum();
    for (int nn=sN; nn<N; nn++) {
    	new_node = G.new_node();
        if (my_random01() < P) {
	  //cout << "Case 1" << endl;
	  // 1 new
	  do {u = pick(); } while (deg[u]>=nn-2); 
          G.new_edge(node_v[u], new_node);
	  // 2 old
	  do { v = pick(); } while(edge_exist(u, v));
	  do { w = pick(); } while(edge_exist(u, w));
          //update Psum
	  deg.push_back(1); Psum += pi[1];
	  deg[u] +=3; Psum += pi[deg[u]]-pi[deg[u]-3];
	  deg[v] ++; Psum += pi[deg[v]]-pi[deg[v]-1];
	  deg[w] ++; Psum += pi[deg[w]]-pi[deg[w]-1];
       } else {
	  //cout << "Case 2" << endl;
	  // 2 new
	  do { u = pick(); v = pick(); 
	  } while (u == v or ( deg[u]==(nn-1) and deg[v]==(nn-1) )); 
          G.new_edge(node_v[u], new_node);
          G.new_edge(node_v[v], new_node);
	  // 1 old
	  //pick x from u, v
	  if (my_random01() < pi[deg[u]]/( pi[deg[u]]+pi[deg[v]] )  ) {
            x = u; y = v; } else { y = u; x = v; }
	  if (deg[x]==(nn-1)) { int t = y; y = x; x=t; }
	  do { w = pick(); } while(x == w or edge_exist(x, w));
          //update Psum
       	  deg.push_back(2); Psum += pi[2];
	  deg[x] +=2; Psum += pi[deg[x]]-pi[deg[x]-2];
	  deg[y] ++; Psum += pi[deg[y]]-pi[deg[y]-1];
	  deg[w] ++; Psum += pi[deg[w]]-pi[deg[w]-1];
       } 
       node_v.push_back(new_node);
    }
    int end = node_v.size(); int max=0;
    for (int i = 0; i < end; ++i) { if(max<deg[i]) {max = deg[i];} }
    cout << "Max " << max << endl;
    //printtopo();
}
void PFP::gen_pfp2()
{
    int u, v, w, x, y ;
    node new_node;
    G.make_undirected();
    gen_rn(sN, sM);
    Psum=cal_Psum();
    for (int nn=sN; nn<N; nn++) {
    	new_node = G.new_node();
        if (my_random01() < P) {
	  //cout << "Case 1" << endl;
	  // 1 new
	  do {u = pick(); } while (deg[u]>=nn-2); 
          G.new_edge(node_v[u], new_node);
	  // 2 old
          do { v = pick(); w = pick(); } while(edge_exist(v, w));
	  do { x = pick(); y = pick(); } while(edge_exist(x, y));
          //update Psum
	  deg.push_back(1); Psum += pi[1];
	  deg[u]++; Psum += pi[deg[u]]-pi[deg[u]-1];
	  deg[v]++; Psum += pi[deg[v]]-pi[deg[v]-1];
	  deg[w]++; Psum += pi[deg[w]]-pi[deg[w]-1];
	  deg[x]++; Psum += pi[deg[x]]-pi[deg[x]-1];
	  deg[y]++; Psum += pi[deg[y]]-pi[deg[y]-1];
       } else {
	  //cout << "Case 2" << endl;
	  // 2 new
	  do { u = pick(); v = pick(); 
	  } while (u == v or  deg[u]==(nn-1) or deg[v]==(nn-1) ); 
          G.new_edge(node_v[u], new_node);
          G.new_edge(node_v[v], new_node);
	  // 1 old
	  do { w = pick(); x = pick(); } while(edge_exist(w, x));
          //update Psum
       	  deg.push_back(2); Psum += pi[2];
	  deg[u]++; Psum += pi[deg[u]]-pi[deg[u]-1];
	  deg[v]++; Psum += pi[deg[v]]-pi[deg[v]-1];
	  deg[w]++; Psum += pi[deg[w]]-pi[deg[w]-1];
	  deg[x]++; Psum += pi[deg[x]]-pi[deg[x]-1];
       } 
       node_v.push_back(new_node);
    }
    int end = node_v.size();
    int max=0;
    for (int i = 0; i < end; ++i) { if(max<deg[i]) {max = deg[i];} }
    cout << "Max " << max << endl;
    //printtopo();
}
void PFP::printtopo()
{
    graph::edge_iterator start, end;

    end = G.edges_end();
    for (start = G.edges_begin(); start != end; start++) {
        cout << (start->source()).id()  << " "
            << (start->target()).id()  << endl;
    }
}


