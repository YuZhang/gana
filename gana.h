#ifndef __GANA_H
# define __GANA_H

#include "utils.h"
#include "coreness.h"
#include "sp_based.h"
#include "degree_based.h"
#include "clustering.h"
#include "stat.h"
#include "pfp.h"
#include "rewire.h"
#include "match.h"

extern bool savetopo(graph & G, const char *filename);
extern bool loadtopo(graph & G, const char *filename);
extern void make_connected(graph & G);
extern bool convert_measurement_results_to_topo(graph & G,
                                                const char *filename);
//extern void sp_based(graph & G);
extern void block_cutpoints(graph & G);

#endif
