#ifndef _PFP_H
# define _PFP_H
#include "utils.h"
class PFP {
public:
    PFP(graph &, int, int);
    void gen_pfp();
    void gen_pfp2();
    void printtopo();
private:
    int N ;
    int sN ;
    int sM ;
    double P;
    double D;
    double Psum ;
    graph & G;

    vector < node > node_v;
    vector <int> deg;
    vector <double> pi;
    
    int edge_exist(int, int);
    void gen_rn(int, int);
    double cal_Psum();
    double cal_Psum2();
    int pick();
};

#endif
