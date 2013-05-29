
#include "utils.h"

bool loadtopo(graph & G, const char *filename)
{
    int start, end, max;
    char c;
    int zero=0;
    vector < node > node_v;
    node start_node, end_node;

    ifstream ifs(filename);
    if (!ifs) {
        cerr << "error open file" << filename << endl;
        return false;
    }
    G.make_undirected();
    ifs >> noskipws;
    while (ifs >> start >> c) {
	if (start==0) zero=1;
        do {
        ifs >> end >> c;
        edge tmp_edge;
        max = start > end ? start : end;
        for (int i = node_v.size(); i < max+zero; i++) {
            node tmp_node = G.new_node();
            node_v.push_back(tmp_node);
        }
	//if (end<start) continue;
        start_node = node_v[start-(1-zero)];
        end_node = node_v[end-(1-zero)];
        tmp_edge = G.new_edge(start_node, end_node);
	} while (c==' ');
    }
    ifs.close();
    return true;
}

bool savetopo(graph & G, const char *filename)
{
    ofstream ofile(filename);
    graph::edge_iterator start, end;

    if (!ofile) {
        cerr << "error open file" << filename << endl;
        return false;
    }
    end = G.edges_end();
    for (start = G.edges_begin(); start != end; start++) {
        ofile << (start->source()).id()  << " "
            << (start->target()).id()  << endl;
    }
    return true;
}

static vector < edge > isadd_v;

static bool load_measurement_results(graph & G, const char *filename)
{
    int start, end, max;
    // int isadd;
    ifstream ifs(filename);
    node start_node, end_node;
    vector < node > node_v(1);

    if (!ifs) {
        cerr << "error open file" << filename << endl;
        return false;
    }
    G.make_undirected();
    while (ifs >> start) {
        ifs >> end;
        //ifs>> isadd;
        edge tmp_edge;
        max = start > end ? start : end;
        for (int i = node_v.size() - 1; i < max; i++) {
            node tmp_node = G.new_node();
            node_v.push_back(tmp_node);
        }
        start_node = node_v[start];
        end_node = node_v[end];
        tmp_edge = G.new_edge(start_node, end_node);
        //if (isadd == 1) {
        //    isadd_v.push_back(tmp_edge);
        //}
    }
    cout << "Graph is connected ? " << (G.is_connected() ==
                                        true ? "true" : "false")
        << endl;
    vector < edge >::iterator estart, eend;
    eend = isadd_v.end();
    for (estart = isadd_v.begin(); estart != eend; estart++) {
        G.hide_edge(*estart);
    }
    ifs.close();
    return true;
}

void make_connected(graph & G)
{
    components compo;
    unsigned int biggest_compo_size = 0;
    node_map < int >on_component(G, 0);
    components::component_iterator start, end, biggest;
    vector < list < node > *>id2compo;

    /* Run component */
    cout << "checking whether components can be applied            ";
    if (compo.check(G) != algorithm::GTL_OK) {
        cout << "failed" << endl;
        return;
    }
    cout << "suceeded" << endl;
    cout << "Running component:                                    ";
    if (compo.run(G) != algorithm::GTL_OK) {
        cout << "failed" << endl;
        return;
    }
    cout << "suceeded" << endl;
    cout << "The number of components: " << compo.
        number_of_components() << endl;
    /* end of running component */

    /* assign component id to each node */
    end = compo.components_end();
    int compo_id = 0;
    for (start = compo.components_begin(); start != end; start++) {
        list < node >::iterator nstart, nend;
        cout << "The size of component [id " << compo_id << "] : "
            << start->first.size() << endl;
        id2compo.push_back(&(start->first));
        nend = start->first.end();
        for (nstart = start->first.begin(); nstart != nend; nstart++) {
            on_component[*nstart] = compo_id;
            cout << "Node " <<*nstart<< " degree "<< nstart->degree() << endl;
        }
        compo_id++;
    }
    /* end of assigning */

    /* joint components */
    vector < edge >::iterator estart, eend;
    eend = isadd_v.end();
    for (estart = isadd_v.begin(); estart != eend; estart++) {
        int sid, eid;
        sid = on_component[estart->source()];
        eid = on_component[estart->target()];
        if (sid != eid) {
            G.restore_edge(*estart);
            cout << "joint components  " << sid << " " << eid << endl;
            list < node >::iterator nstart, nend;
            if (sid < eid) {
                nend = id2compo[eid]->end();
                for (nstart = id2compo[eid]->begin(); nstart != nend;
                     nstart++) {
                    on_component[*nstart] = sid;
                }
            } else {
                nend = id2compo[sid]->end();
                for (nstart = id2compo[sid]->begin(); nstart != nend;
                     nstart++) {
                    on_component[*nstart] = eid;
                }
            }
        }
        /* next addtional edge */
    }
    /* end of jointing components */

    /* Run component again */
    compo.reset();
    cout << "Running component:                                    ";
    if (compo.run(G) != algorithm::GTL_OK) {
        cout << "failed" << endl;
        return;
    }
    cout << "suceeded" << endl;
    cout << "The number of components: " << compo.
        number_of_components() << endl;
    /* end of running component */

    /* find biggest component */
    biggest_compo_size = 0;
    for (start = compo.components_begin(); start != end; start++) {
        list < node >::iterator nend;
        nend = start->first.end();
        cout << "The size of component: " << start->first.size() << endl;
        if (biggest_compo_size < start->first.size()) {
            biggest_compo_size = start->first.size();
            biggest = start;
        }
    }
    /* end of finding */
    cout << "The biggest component size is " << biggest_compo_size << endl;

    /* Add edges to joint components */
    for (start = compo.components_begin(); start != end; start++) {
        cout << "component: nodes " << start->first.size() << " edges " <<
            start->second.size() << endl;
        list < node >::iterator nend;
        nend = start->first.end();
        if (biggest != start) {
            G.new_edge(*(start->first.begin()), *(biggest->first.begin()));
            cout << "Joint component by adding edges" << endl;
        }
    }
    /* end of adding edges */
    return;
}

bool convert_measurement_results_to_topo(graph & G, const char *filename)
{
    if (load_measurement_results(G, filename) == false) {
        cerr << "Could not load file " << string(filename);
        return false;
    }
    make_connected(G);
    string outfilename = string(filename) + string(".topo");
    return savetopo(G, outfilename.c_str());
}

void find_biggest_component(graph & G, graph & B)
{
    components compo;
    unsigned int biggest_compo_size = 0;
    node_map < int >on_component(G, 0);
    components::component_iterator start, end, biggest;
    vector < list < node > *>id2compo;

    /* Run component */
    if (compo.check(G) != algorithm::GTL_OK) {
        cout << "!checking whether components can be applied failed" << endl;
        return;
    }
    if (compo.run(G) != algorithm::GTL_OK) {
        cout << "!Running components failed" << endl;
        return;
    }
    cout << "The number of components: " << compo.
        number_of_components() << endl;
    /* end of running component */

    /* find biggest component */
    biggest_compo_size = 0;
    for (start = compo.components_begin(); start != end; start++) {
        list < node >::iterator nend;
        nend = start->first.end();
        cout << "component: nodes " << start->first.size() << " edges " <<
            start->second.size() << endl;
        if (biggest_compo_size < start->first.size()) {
            biggest_compo_size = start->first.size();
            biggest = start;
        }
    }
    /* end of finding */
    cout << "The biggest component size is " << biggest_compo_size << endl;
    graph(B, biggest->first);
    return;
}
