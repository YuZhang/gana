#include "degree_based.h"
#include "stat.h"

K_based::K_based(const graph& G):g(G), deg(G,0)
{
    K1();
    K2();
}
void K_based::K1()
{
    double nrn = g.number_of_nodes();
    double other =0;
    graph::node_iterator it, end;
    end = g.nodes_end();
    for (it = g.nodes_begin(); it != end; ++it) {
        double d = it->degree();
	deg[*it]=d;
	if ( d<1e-10) {cout << ">>>> " << *it << endl;}
        if (pk.count(d))
            pk[d] += 1;
        else
            pk.insert(make_pair(d, 1));
    }
    data_t::iterator dit, dend, max;
    dend=pk.end();
    max = pk.begin();
    for (dit=pk.begin();dit!=dend;++dit) {
        entropy += (-1.0)*dit->second/nrn*(log(dit->second/nrn));
	pk[dit->first]=dit->second/nrn;
	if (dit->first < 5) {
		other+= dit->second;
		cout << "Degree " << dit->first << " " << dit->second << endl;
	}
	if (dit->first > max->first) {
		max = dit;
	}
    }
    cout << "Degree other " << 1-other << endl;
		cout << "Max degree: " << max->first << endl;

}
void K_based::K2()
{
    double nre = g.number_of_edges();
    double m = 0, s = 0, ss = 0;
    //double nrl = G.number_of_edges();
    graph::edge_iterator it, end;
    end = g.edges_end();
    for (it = g.edges_begin(); it != end; ++it) {
        double d1 = deg[it->source()];
        double d2 = deg[it->target()];
        if (jdd.count(make_pair(d1, d2))) {
               jdd[make_pair(d1, d2)]+=1;
               jdd[make_pair(d2, d1)]+=1;
        } else {
           if (d1==d2) {
               jdd.insert(make_pair(make_pair(d1, d2),2));
           } else {
               jdd.insert(make_pair(make_pair(d1, d2),1));
               jdd.insert(make_pair(make_pair(d2, d1),1));
           }
        }
	m += d1 * d2;
        s += d1 + d2;
        ss += d1 * d1 + d2 * d2;

    }
    data3_t::iterator dit, dend;
    dend = jdd.end();
    for (dit=jdd.begin(); dit!=dend; ++dit) {
       double k1=dit->first.first;
       double k2=dit->first.second;
       if (knn.count(k1)==0) {
           knn.insert(make_pair(k1, 0));
       }
       knn[k1] += k2*(dit->second/nre)/(pk[k1]*k1);
    }

    ac = ss / nre / 2 - s * s / nre / nre / 4;
    if (ac <= 1e-10) {
        ac = 1;
    } else {
        ac = (m / nre - s * s / nre / nre / 4) / ac;
    }
}


ostream& operator<<(ostream& os, const K_based& kb)
{
    os << "Node degree:" << endl;
    print_node_map(kb.g, kb.deg);
    os << "#" << endl;
    Dstat ds0(kb.pk);
    double avgd = kb.g.number_of_edges()*2.0/kb.g.number_of_nodes();
    os << "Average degree: " << kb.g.number_of_nodes() 
       << " " << kb.g.number_of_edges() <<" " << avgd << endl;

    os << "Node degree distribution:" << endl;
    ds0.print();
    os << "#" << endl;
    os << "Degree distribution ";
    ds0.linefit(6,40,'z', true);
    /*int i=1; int j=0;
       for (; i<10; ++i) 
       for (j=i+10; j<100; ++j) {
       linefit(results, i, j, 'z', true, s, c);
       if (c > 0.999) {
       cout << "= " << i << " " << j << " > ";
       cout << s-1 << " " << c << endl;
       }
       } */

   os << "Entropy of degree distribution: " << kb.entropy << endl;
   os << "Assortative coefficient: " << kb.ac << endl;
   
   Dstat ds1(kb.knn);
   os << "Average neighbors' degree:" << endl;
   ds1.print();
   os << "#" << endl;
   os << "Average neighbors' degree ";
   ds1.linefit(6, 40, 'z', false);
   return os;
}
