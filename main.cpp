#include "gana.h"
using namespace std;

void usage(const string & additional)
{
    cout << additional << endl;
    cout << "Usage: gana <GRAPH File>" << endl;
}

void characterize(graph & G) 
{   
    K_based kb(G);
    cout << kb;
    Richclub rc(G);
    cout << rc;
    Coreness cn(G);
    cout << cn;
    Cluster cs(G);
    cout << cs;
    Sp_based sp(G);
    cout << sp;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        usage("Please specify a file with a graph");
        exit(1);
    }

    graph G;
    //convert_measurement_results_to_topo(G, argv[1]);
    //cout << "IS CONNECTED? " << (G.is_connected()?"True":"False") << endl;
    //return 0; 
    if (loadtopo(G, argv[1]) == false) {
        usage(string("Could not load file ") + string(argv[1]));
        exit(1);
    }
    cout << "== 0 ==" << endl;
    characterize(G);
//  graph H(G);
//  for(int i=1; i<=1000; ++i) {
//    out << "== " << i << " ==" << endl;
//    MATCH match(H, i, 2);
//    REWIRE rewire(H, i, 1000, 1);
//    characterize(H);
//  }
    return 0;
}
