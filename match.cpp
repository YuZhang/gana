#include "match.h" 
#include "random.h"
static KW_RNG::RNG _my_random;
static void my_srandom(int x) { _my_random.init(x,!x*13,x*x+1,(x>>16)+(x<<16)); }
static double my_random01() { return _my_random.rand_halfclosed01(); }

static bool compare_by_degree(node a, node b)
{
    return (a.degree() > b.degree());
}

MATCH::MATCH(graph & g, int s, int d): G(g), D(d)
{
   my_srandom(s);
   vector< node > nv;
   node_map<int> nm(G, 0);
   graph::node_iterator it, end = G.nodes_end();
   for(it=G.nodes_begin(); it!=end; ++it) {
       nv.push_back(*it);
       nm[*it]=it->degree();
   }
   stable_sort(nv.begin(), nv.end(), compare_by_degree);
   list<edge> el=G.all_edges();
   list<edge>::iterator ei, ee = el.end();
   for(ei=el.begin(); ei!=ee; ++ei) {
      G.del_edge(*ei);
   }
   int nrn = G.number_of_nodes();
   switch(D) {
   case 1: {
     for(int i=0; i<nrn-1; ++i) {
       int k=i+1;
       for(int j=nm[nv[i]]; j>0; --j) {
         while (nm[nv[k]]==0 and k<nrn) { k++; }
	 if(k==nrn) {return;}
         G.new_edge(nv[i], nv[k]);
         nm[nv[k]]--;
	 k++;
       }
     }
     break; }
   case 2: {
     int l=nrn-1;
     for(int i=0; i<nrn; ++i) {
       int k=l;
       for(int j=nm[nv[i]]; j>0; --j) {
         while (nm[nv[k]]==0 and k>0) {
           k--; 
           cout << "= " << i << " " << j << " " << k << endl;
	   if (k==i or k==0) {return;}
	 }
         G.new_edge(nv[i], nv[k]);
         nm[nv[k]]--;
	 if(nm[nv[k]]==0 and k<l) {l=k;}
	 k--;
       }
       nm[nv[i]]=0;
     }
     cout << "===" << endl;
     break; }
   default: {
     break; }
  }
}

