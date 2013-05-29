#include "sp_based.h"

Sp_based::Sp_based(const graph& G) :
    og(G), g(G), inQ(G, 0), tt(G), on_compo(G),coms(G), n_stub(G, 0), dd(G, 0),
    d_stub(G), avgd(G,0), maxd(G,0), nbet(G,0), ebet(g,0)
{
    id2compo();
    shrink();
    shortest_path();
}

void Sp_based::shortest_path()
{
    graph::node_iterator sit, send;
//double nrn = g.number_of_nodes();
//cout << setfill(' ') << right;
    send = g.nodes_end();
    int i=0;
    for (sit = g.nodes_begin(); sit != send; ++sit) {
        i++;
        node s = *sit;
//        cout << "\rsp on " << setw(8) << i << "/" << nrn <<flush;
        queue < node > Q;
        Q.push(s);              /* Dijkstra SP */
        node_map < unsigned int >d(g, 0xffffffff);
        d[s] = 0;
        stack < node > S;       /* betweenness */
        node_map < list < edge > >P(g); /* betweenness */
        node_map < int >N(g, 0);  /* betweenness */
        N[s] = 1;  
        while (Q.empty() == false) {
            node v = Q.front();
            Q.pop();
            S.push(v);          /* betweenness */
            node::adj_edges_iterator eit, eend;
            eend = v.adj_edges_end();
            for (eit = v.adj_edges_begin(); eit != eend; ++eit) {
                node w = eit->target();
                if (w == v) 
                    w = eit->source();
                if (d[w] == 0xffffffff) {
                    Q.push(w);
                    d[w] = d[v] + 1;
                }
                if (d[w] == d[v] + 1) {
                    N[w] += N[v];
                    P[w].push_back(*eit);
                }
            }
        }
	maxd[s]=maxd[s]>d[S.top()]?maxd[s]:d[S.top()];
        nm_t D(g, 0);
	vector<int> idd((int)maxd[s]+1,0);
        while (S.empty() == false) {
            node w = S.top();
            S.pop();
	    // for shrink
	    if(w==s) continue;
            unsigned int l = d[w];
            avgd[s] += l*(1+n_stub[w])+dd[w];
	    if (maxd[s] < (l+d_stub[w].size()-1)) {
                maxd[s] = l+d_stub[w].size()-1;
	        idd.resize((int)maxd[s]+1,0);
	    }
	    for (unsigned int i=1; i<d_stub[w].size(); ++i) {
                if (inQ[s]) idd[l+i]+=d_stub[w][i];
                pairs[l+i]+=d_stub[w][i];
	    }

	    if (inQ[s]) idd[l]++;    
            pairs[l]++;
	    // for shrink
            list < edge >::iterator eit, eend;
            eend = P[w].end();
            for (eit = P[w].begin(); eit != eend; ++eit) {
                node v = eit->source();
                if (w==v) v = eit->target();
                double t = ((double)N[v]/(double)N[w])*(1+D[w]+n_stub[w]);
                ebet[*eit] += t*(n_stub[s]+1);
		if (s!=v) D[v] += t;
            }
            nbet[w] += D[w]*(n_stub[s]+1);
        }
        if (!inQ[s]) continue;
        tree(s);
	for (unsigned int i=1; i<d_stub[s].size(); ++i) {
	    for (unsigned int j=1; j<idd.size(); ++j) {
	        if(idd[j]==0) continue;
			pairs[i+j]+=idd[j]*d_stub[s][i];
	    }
	}
    }
//    cout << endl;
    g.restore_graph();
    for (sit = g.nodes_begin(); sit != send; ++sit) {
        avgd[*sit]/=(coms[*sit]-1);
    }
}

void Sp_based::tree(const node& n)
{
    for (unsigned int i=1; i<d_stub[n].size(); ++i)
             pairs[i]+=d_stub[n][i]*2;
    list<node>::iterator it, end;
    end = tt[n].end();
    for (it=tt[n].begin(); it!=end; ++it) {
	node v = *it;
        avgd[v]=avgd[n]+coms[v]-2*(n_stub[v]+1);
        unsigned int m = 1;
        for (unsigned int i=1; i<d_stub[n].size() ; ++i) {
            for (unsigned int j=0; j<d_stub[v].size() ; ++j) {
		int k = d_stub[n][i]-(d_stub[v].size()>(i-1)?d_stub[v][i-1]:0);
		if (k==0) continue;
		pairs[i+j+1]+= k*d_stub[v][j];
		if (m<(i+1)) m=i+1;
	    }
	}
	maxd[v]=(maxd[n]+1)>m?(maxd[n]+1):m;
	maxd[v]=maxd[v]>(d_stub[v].size()-1)?maxd[v]:(d_stub[v].size()-1);
	tree(v);
    }
    maxd[n]=maxd[n]>(d_stub[n].size()-1)?maxd[n]:(d_stub[n].size()-1);
}

void Sp_based::shrink()
{
    queue < node > Q;
    node lastpush;
    int cc=0;
    graph::node_iterator it, end;
    end = g.nodes_end();
    for (it=g.nodes_begin(); it!=end; ++it) {
        d_stub[*it].push_back(1);
        if (it->degree()==1) {
            Q.push(*it);
	    inQ[*it]=1;
	}
    }
    while (!Q.empty()) {
        node v = Q.front();
        Q.pop();
	inQ[v]=0;
	if (v.degree()==0) {
            g.hide_node(v);
	    avgd[v]=dd[v];
	    if(lastpush==v) cc=1;
	    tree(v);
            continue;
	}
	if (v.degree()>1) {
	    Q.push(v);
	    inQ[v]=1;
	    if (lastpush==v) break;
	    if(cc) {lastpush=v; cc=0;}
            continue;
        }
	if(cc) cc=0;
	node w = *(v.adj_nodes_begin());
	if (!inQ[w]) {
	    Q.push(w);
	    inQ[w]=1;
	}
	tt[w].push_back(v);
	lastpush=w;
	dd[w] += n_stub[v]+dd[v]+1;
        n_stub[w] += n_stub[v]+1;
        nbet[w] += (n_stub[v]+1)* (coms[v] - n_stub[v] - 2);
	nbet[v] += n_stub[v] * (coms[v] - n_stub[v] - 1);
	edge e = *(v.adj_edges_begin());
        ebet[e] = (n_stub[v]+1)* (coms[v] - n_stub[v] - 1)*2;
	for (unsigned int i=0; i<d_stub[v].size(); ++i) {
            if (i+2 > d_stub[w].size())
		 d_stub[w].push_back(0);   
	    d_stub[w][i+1]+=d_stub[v][i];
	}
        g.hide_node(v);
    }
    while (!Q.empty()) {
        node v = Q.front();
	Q.pop();
	nbet[v] += n_stub[v] * (coms[v] - n_stub[v] - 1);
	avgd[v]=dd[v];
    }
}

void Sp_based::id2compo()
{
    components::component_iterator start, end, biggest;

    if (compo.check(g) != algorithm::GTL_OK) {
        cout << "!Check components: failed" << endl;
        return;
    }
    if (compo.run(g) != algorithm::GTL_OK) {
        cout << "!Running component: failed" << endl;
        return;
    }
    cout << "The # of components: " << compo.number_of_components() << endl;

    start = compo.components_begin();
    end = compo.components_end();
    for (biggest=start; start != end; start++) {
        list < node >::iterator nstart, nend;
	if ((biggest->first.size()) < (start->first.size())) {
           biggest = start;
        }
        nend = start->first.end();
        for (nstart = start->first.begin(); nstart != nend; nstart++) {
            on_compo[*nstart] = start;
            coms[*nstart] = start->first.size();
        }
    }
}

ostream& operator<<(ostream& os, const Sp_based& spb)
{    
    double total=0;	
    double sum=0;
    double avg=0;
    double stdvar=0;
    double max=0;
    os << "Pair_Hop:" << endl;
    os << "0 " << spb.og.number_of_nodes() << endl;
    data_t::const_iterator it, end;
    end = spb.pairs.end();
    for (it = spb.pairs.begin(); it != end; ++it) {
	total += it->second/2;
	sum += (it->first)*(it->second)/2;
	if(max < it->first) {max=it->first;}
    }
    avg = sum/total;
    for (it = spb.pairs.begin(); it != end; ++it) {
        os << it->first << " " << it->second << endl;
        stdvar += pow((it->first)-avg,2)*(it->second/2/total);
    }
    os << "#" << endl;
    os << "Average distance: " << avg << endl;
    os << "Stdv of distance: " <<  sqrt(stdvar) << endl;
    os << "Node max distance: " << max << endl;
    print_node_map(spb.og, spb.maxd);

    os << "Node average distance:" << endl;
    print_node_map(spb.og, spb.avgd);
    os << "#" << endl;
    os << "Node max distance:" << endl;
    print_node_map(spb.og, spb.maxd);
    os << "#" << endl;
    os << "Node betweenness:" << endl;
    print_node_map(spb.og, spb.nbet);
    os << "#" << endl;
    os << "Edge betweenness:" << endl;
    print_edge_map(spb.g, spb.ebet);
    os << "#" << endl; 
    return os;
}

