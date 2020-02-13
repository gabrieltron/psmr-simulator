#ifndef MODEL_MIN_CUT_H
#define MODEL_MIN_CUT_H

#include <algorithm>
#include <float.h>
#include <fstream>
#include <math.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "graph.h"
#include "metis.h"
#include "partition_scheme.h"
#include "spanning_tree.h"

namespace model {

enum CutMethod {METIS, FENNEL};
const std::unordered_map<std::string, CutMethod> string_to_cut_method({
    {"METIS", METIS},
    {"FENNEL", FENNEL}
});
const double MAX_IMBALANCE = 1.03;

workload::PartitionScheme cut_graph(
    CutMethod cut_method, Graph& graph, idx_t n_partitions
);

workload::PartitionScheme metis_cut(Graph& graph, idx_t n_partitions);

workload::PartitionScheme fennel_cut(Graph& graph, int n_partitions);
int fennel_inter_cost(
    const std::unordered_map<int, int>& edges,
    workload::Partition& partition
);
int fennel_vertice_partition(
    Graph& graph,
    int vertice,
    std::vector<workload::Partition>& partitions,
    double gamma
);
int biggest_value_index(std::vector<double>& partitions_score);

workload::PartitionScheme spanning_tree_cut(SpanningTree& tree, int n_partitions);

}

#endif
