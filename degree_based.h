#ifndef __K_BASED_H
# define __K_H
#include "utils.h"
class K_based
{
    friend ostream& operator<<(ostream&, const K_based&);
public:
    K_based(const graph&);
private:
    const graph& g;
    double entropy;
    nm_t deg;
    data_t pk;
    data_t knn;
    data3_t jdd;
    double ac;
    void K1();
    void K2();
};
#endif
