#include "partition_manager.h"

namespace workload {

PartitionManager::PartitionManager(
    int n_partitions,
    const std::vector<int>& values
) : round_robin_counter_{0} {

    partitions_ = std::vector<Partition>(n_partitions);
    for (const auto& value: values) {
        partitions_.at(round_robin_counter_).insert(value);
        round_robin_counter_ = (round_robin_counter_+1) % n_partitions;
    }
}

PartitionManager::PartitionManager(
    std::vector<Partition>& partitions
) : partitions_{partitions}
{
    update_partitions(partitions);
}

PartitionManager::PartitionManager(
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
        value_to_partition_[data] = data_partition;
    }
}

void PartitionManager::update_partitions(std::vector<Partition>& partitions) {
    partitions_ = partitions;

    for (auto i = 0; i < partitions.size(); i++) {
        for (auto vertice : partitions[i].data()) {
            value_to_partition_[vertice] = i;
        }
    }
}

void PartitionManager::add_value(int value, int partition, int n_accesses) {
    partitions_[partition].insert(value, n_accesses);
    value_to_partition_[value] = partition;
}

void PartitionManager::increase_partition_weight(
    int partition_id, int weight/*=1*/
) {
    auto partition = value_to_partition_.at(partition_id);
}

void PartitionManager::remove_value(int value) {
    auto values_partition = value_to_partition_.at(value);
    partitions_.at(values_partition).remove(value);
    value_to_partition_.erase(value);
}

bool PartitionManager::in_scheme(int value) const {
    return value_to_partition_.find(value) != value_to_partition_.end();
}

int PartitionManager::n_partitions() const {
    return partitions_.size();
}

int PartitionManager::value_to_partition(int value) const {
    return value_to_partition_.at(value);
}

const std::vector<Partition>& PartitionManager::partitions() const {
    return partitions_;
}

const std::unordered_map<int, int>&
PartitionManager::value_to_partition_map() const {
    return value_to_partition_;
}

model::Graph PartitionManager::graph_representation() const {
    auto graph = model::Graph();

    for(auto value = 0; value < value_to_partition_.size(); value++) {
        auto partition = value_to_partition_.at(value);

        graph.add_vertice(value);
        for (const auto& neighbour: (partitions_.at(partition)).data()) {
            if (neighbour == value) {
                continue;
            }

            graph.add_edge(value, neighbour);
            graph.add_edge(neighbour, value);
        }
    }

    return graph;
}

}
