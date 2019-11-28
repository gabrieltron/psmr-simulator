#include "partition_scheme.h"

namespace workload {

PartitionScheme::PartitionScheme(
    std::vector<long int>& data_partitions
) : data_partitions_{data_partitions}
{
    update_partitions(data_partitions);
}

void PartitionScheme::update_partitions(
    const std::vector<long int>& data_partitions
) {
    data_partitions_ = data_partitions;
    partitions_ = Partitions();

    for (auto data = 0; data < data_partitions.size(); data++) {
        auto partition = data_partitions[data];
        if (partitions_.find(partition) == partitions_.end()) {
            partitions_[partition] = std::unordered_set<int>();
        }

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

}