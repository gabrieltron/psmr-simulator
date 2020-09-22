#ifndef MODEL_PARTITION_SCHEME_H
#define MODEL_PARTITION_SCHEME_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph/graph.h"
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

    void add_data(int data, int partition, int data_size);
    void increase_partition_weight(int data, int weight = 1);
    void remove_data(int data);
    void update_partitions(std::vector<Partition>& partitions);
    bool in_scheme(int data) const;

    model::Graph graph_representation();
    std::vector<Partition>& partitions();
    int data_partition(int data);
    const std::unordered_map<int, int>& data_partition_map();
    int n_partitions();

private:
    std::unordered_map<int, int> data_partitions_;
    std::vector<Partition> partitions_;
};

}

#endif
