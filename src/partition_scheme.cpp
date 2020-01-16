#include "partition_scheme.h"

namespace workload {

PartitionScheme::PartitionScheme(
    int n_partitions,
    std::vector<idx_t>& data_partitions
) : data_partitions_{data_partitions}
{
    update_partitions(n_partitions, data_partitions);
}

void PartitionScheme::update_partitions(
    int n_partitions,
    const std::vector<idx_t>& data_partitions
) {
    data_partitions_ = data_partitions;
    partitions_ = Partitions();

    for (auto i = 0; i < n_partitions; i++) {
        partitions_[i] = std::unordered_set<int>();
    }

    for (auto data = 0; data < data_partitions.size(); data++) {
        auto partition = data_partitions[data];
        partitions_[partition].insert(data);
    }
}

model::Graph PartitionScheme::graph_representation() {
    auto graph = model::Graph();

    for(auto data = 0; data < data_partitions_.size(); data++) {
        auto partition = data_partitions_[data];

        graph.add_vertice(data);
        for (auto neighbour: partitions_[partition]) {
            if (neighbour == data) {
                continue;
            }

            graph.add_edge(data, neighbour);
            graph.add_edge(neighbour, data);
        }
    }

    return graph;
}

Partitions PartitionScheme::partitions() {
    return partitions_;
}

int PartitionScheme::data_partition(int data) {
    return data_partitions_[data];
}

std::vector<idx_t> PartitionScheme::data_partition_vector() {
    return data_partitions_;
}

int PartitionScheme::n_partitions() {
    return partitions_.size();
}

}