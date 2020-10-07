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

#include "kaHIP_interface.h"
#include "metis.h"

#include "graph/graph.h"
#include "graph/spanning_tree.h"
#include "partition/partition_scheme.h"

namespace model {

enum CutMethod {METIS, KAHIP, FENNEL, REFENNEL};
const std::unordered_map<std::string, CutMethod> string_to_cut_method({
    {"METIS", METIS},
    {"KAHIP", KAHIP},
    {"FENNEL", FENNEL},
    {"REFENNEL", REFENNEL}
});
const double MAX_IMBALANCE = 1.03;

workload::PartitionScheme multilevel_cut(
    Graph& graph, int n_partitions, CutMethod cut_method
);
workload::PartitionScheme fennel_cut(Graph& graph, int n_partitions);
workload::PartitionScheme refennel_cut(
    Graph& graph, workload::PartitionScheme& old_partition
);
workload::PartitionScheme spanning_tree_cut(SpanningTree tree, int n_partitions);

}

#endif
