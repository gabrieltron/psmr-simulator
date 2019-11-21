#ifndef MODEL_GRAPH_CUT_H
#define MODEL_GRAPH_CUT_H

#include <vector>

#include "graph.h"
#include "metis.h"
#include "partition_scheme.h"

namespace model {

PartitionScheme cut_graph(Graph& graph, idx_t n_partitions);

}

#endif
