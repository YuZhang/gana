#include "utils.h"
void block_cutpoints(graph & G)
{
    biconnectivity bicon;
    biconnectivity::cutpoint_iterator start, end;
    biconnectivity::component_iterator cstart, cend;

    bicon.store_components(true);
    cout << "checking whether biconnectivity can be applied            ";
    if (bicon.check(G) != algorithm::GTL_OK) {
        cout << "failed" << endl;
        return;
    }
    cout << "suceeded" << endl;
    cout << "Running biconnectivity:                                   ";
    if (bicon.run(G) != algorithm::GTL_OK) {
        cout << "failed" << endl;
        return;
    }
    cout << "suceeded" << endl;
    int cutp_c = 0;
    end = bicon.cut_points_end();
    for (start = bicon.cut_points_begin(); start != end; start++) {
        //cout << start->id()+1 << endl;
        cutp_c++;
    }
    cout << "The number of cutpointss: " << cutp_c << endl;
    cout << "The number of blocks: " << bicon.number_of_components() << endl;
    cend = bicon.components_end();
    int compo_id = 0;
    for (cstart = bicon.components_begin(); cstart != cend; cstart++) {
        list < node >::iterator nstart, nend;
        cout << "The size of component [id " << compo_id << "] : "
            << cstart->first.size() << endl;
        //id2compo.push_back(&(start->first));
        nend = cstart->first.end();
        for (nstart = cstart->first.begin(); nstart != nend; nstart++) {
            //on_component[*nstart] = compo_id;
            //cout << "Node " << nstart->id()+1 
            //     << " degree " << nstart->degree() << endl;
        }
        compo_id++;
    }

    return;
}
