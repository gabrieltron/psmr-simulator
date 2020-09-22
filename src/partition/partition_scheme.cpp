#include "partition_scheme.h"

namespace workload {

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
        data_partitions_[data] = data_partition;
    }
}

void PartitionScheme::update_partitions(std::vector<Partition>& partitions) {
    partitions_ = partitions;

    for (auto i = 0; i < partitions.size(); i++) {
        for (auto vertice : partitions[i].data()) {
            data_partitions_[vertice] = i;
        }
    }
}

void PartitionScheme::add_data(int data, int partition, int data_size) {
    partitions_[partition].insert(data, data_size);
    data_partitions_[data] = partition;
}

void PartitionScheme::increase_partition_weight(int data, int weight /* = 1 */) {
    auto partition = data_partitions_.at(data);
    partitions_.at(partition).increase_weight(data, weight);
}

void PartitionScheme::remove_data(int data) {
    auto data_partition = data_partitions_[data];
    partitions_[data_partition].remove(data);
    data_partitions_.erase(data);
}

bool PartitionScheme::in_scheme(int data) const {
    return data_partitions_.find(data) != data_partitions_.end();
}

model::Graph PartitionScheme::graph_representation() {
    auto graph = model::Graph();

    for(auto data = 0; data < data_partitions_.size(); data++) {
        auto partition = data_partitions_[data];

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
    return data_partitions_[data];
}

const std::unordered_map<int, int>& PartitionScheme::data_partition_map() {
    return data_partitions_;
}

int PartitionScheme::n_partitions() {
    return partitions_.size();
}

}
