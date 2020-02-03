#ifndef MODEL_GRAPH_CUT_H
#define MODEL_GRAPH_CUT_H

#include <float.h>
#include <fstream>
#include <math.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.h"
#include "metis.h"
#include "partition_scheme.h"

namespace model {

enum CutMethod {METIS, FENNEL};
const std::unordered_map<std::string, CutMethod> string_to_cut_method({
    {"METIS", METIS},
    {"FENNEL", FENNEL}
});

workload::PartitionScheme cut_graph(
    CutMethod cut_method, Graph& graph, idx_t n_partitions
);

workload::PartitionScheme metis_cut(Graph& graph, idx_t n_partitions);

workload::PartitionScheme fennel_cut(Graph& graph, int n_partitions);
int fennel_inter_cost(
    std::unordered_map<int, int>& edges,
    workload::Partition& partition
);
int fennel_vertice_partition(
    Graph& graph,
    int vertice,
    std::vector<workload::Partition>& partitions
);
int biggest_value_index(std::vector<double>& partitions_score);

}

#endif
