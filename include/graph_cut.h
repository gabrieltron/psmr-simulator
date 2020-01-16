#ifndef MODEL_GRAPH_CUT_H
#define MODEL_GRAPH_CUT_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "graph.h"
#include "metis.h"
#include "partition_scheme.h"

namespace model {

std::vector<idx_t> cut_graph(Graph& graph, idx_t n_partitions);
std::unordered_map<int, int> partitions_weight(
    Graph& graph, workload::PartitionScheme& partition_scheme
);

}

#endif
