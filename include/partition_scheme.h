#ifndef MODEL_PARTITION_SCHEME_H
#define MODEL_PARTITION_SCHEME_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.h"

namespace workload{

typedef std::unordered_map<int, std::unordered_set<int>> Partitions;

class PartitionScheme {
public:
    PartitionScheme() = default;

    PartitionScheme(
        int n_partitions,
        std::vector<idx_t>& data_partitions
    );

    void update_partitions(
        int n_partitions,
        const std::vector<idx_t>& data_partitions
    );
    model::Graph graph_representation();
    Partitions partitions();
    int data_partition(int data);
    std::vector<idx_t> data_partition_vector();
    int n_partitions();

private:
    std::vector<idx_t> data_partitions_;
    Partitions partitions_;
};

}

#endif