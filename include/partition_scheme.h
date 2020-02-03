#ifndef MODEL_PARTITION_SCHEME_H
#define MODEL_PARTITION_SCHEME_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.h"
#include "partition.h"

namespace workload{

class PartitionScheme {
public:
    PartitionScheme() = default;

    PartitionScheme(
        int n_partitions,
        std::vector<int>& data_partitions
    );

    PartitionScheme(
        std::vector<Partition>& partitions
    );

    void update_partitions(std::vector<Partition>& partitions);
    model::Graph graph_representation();
    std::vector<Partition>& partitions();
    int data_partition(int data);
    std::unordered_map<int, int> data_partition_map();
    int n_partitions();

private:
    std::unordered_map<int, int> data_partitions_;
    std::vector<Partition> partitions_;
};

}

#endif