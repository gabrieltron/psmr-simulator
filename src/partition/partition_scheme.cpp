#include "partition_scheme.h"

namespace workload {

PartitionScheme::PartitionScheme(
    int n_partitions,
    const std::vector<int>& values
) : round_robin_counter_{0} {

    partitions_ = std::vector<Partition>(n_partitions);
    for (const auto& value: values) {
        partitions_.at(round_robin_counter_).insert(value);
        round_robin_counter_ = (round_robin_counter_+1) % n_partitions;
    }
}

PartitionScheme::PartitionScheme(
    std::vector<Partition>& partitions
) : partitions_{partitions}
{
    update_partitions(partitions);
}

PartitionScheme::PartitionScheme(
    int n_partitions, std::vector<int>& data_partitions
) {
    for (auto i = 0; i < n_partitions; i++) {
        auto partition = Partition();
        partitions_.push_back(partition);
    }

    for (auto i = 0; i < data_partitions.size(); i++) {
        auto data = i;
        auto data_partition = data_partitions[i];

        partitions_[data_partition].insert(data, 0);
        data_to_partition_[data] = data_partition;
    }
}

void PartitionScheme::update_partitions(std::vector<Partition>& partitions) {
    partitions_ = partitions;

    for (auto i = 0; i < partitions.size(); i++) {
        for (auto vertice : partitions[i].data()) {
            data_to_partition_[vertice] = i;
        }
    }
}

void PartitionScheme::add_data(int data, int partition, int data_size) {
    partitions_[partition].insert(data, data_size);
    data_to_partition_[data] = partition;
}

void PartitionScheme::increase_partition_weight(int data, int weight /* = 1 */) {
    auto partition = data_to_partition_.at(data);
    partitions_.at(partition).increase_weight(data, weight);
}

void PartitionScheme::remove_data(int data) {
    auto data_partition = data_to_partition_[data];
    partitions_[data_partition].remove(data);
    data_to_partition_.erase(data);
}

bool PartitionScheme::in_scheme(int data) const {
    return data_to_partition_.find(data) != data_to_partition_.end();
}

model::Graph PartitionScheme::graph_representation() {
    auto graph = model::Graph();

    for(auto data = 0; data < data_to_partition_.size(); data++) {
        auto partition = data_to_partition_[data];

        graph.add_vertice(data);
        for (auto neighbour: partitions_[partition].data()) {
            if (neighbour == data) {
                continue;
            }

            graph.add_edge(data, neighbour);
            graph.add_edge(neighbour, data);
        }
    }

    return graph;
}

std::vector<Partition>& PartitionScheme::partitions() {
    return partitions_;
}

int PartitionScheme::data_partition(int data) {
    return data_to_partition_[data];
}

const std::unordered_map<int, int>& PartitionScheme::data_partition_map() {
    return data_to_partition_;
}

int PartitionScheme::n_partitions() {
    return partitions_.size();
}

}
