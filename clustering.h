#ifndef __CLUSTER_H
# define __CLUSTER_H
#include "utils.h"
class Cluster
{
    friend ostream & operator << (ostream&, const Cluster&);
public:
    Cluster(graph&);
private:
    graph& g;
    nm_t nc;
    double cc;
    double tt;

};

class Richclub 
{
    friend ostream & operator << (ostream&, const Richclub&);
public:
    Richclub(graph&);
private:
    graph& g;
    data_t rr, dr;
};

#endif
