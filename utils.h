#ifndef __GANA_UTILS_H
# define __GANA_UTILS_H

#include <stack>
#include <queue>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

#include <GTL/graph.h>
#include <GTL/node.h>
#include <GTL/edge.h>
#include <GTL/components.h>
#include <GTL/biconnectivity.h>

typedef map < double, double >data_t;
typedef map < pair<double, double>, double >data3_t;
typedef node_map< double > nm_t;
typedef edge_map< double > em_t;

void print_node_map(const graph &, const nm_t&);
void print_edge_map(const graph &, const em_t&);
#include "stat.h"
#endif

