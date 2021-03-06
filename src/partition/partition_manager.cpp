#include "partition_manager.h"

namespace workload {

PartitionManager::PartitionManager(
    int n_partitions,
    const std::vector<int>& values
) : round_robin_counter_{0},
    access_graph_{model::Graph()} {

    partitions_ = std::vector<Partition>(n_partitions);
    for (const auto& value: values) {
        allocate_value(value);
        access_graph_.add_vertice(value);
    }
}

PartitionManager::PartitionManager(
    std::vector<Partition>& partitions
) : partitions_{partitions}
{
    update_partitions(partitions);
}

void PartitionManager::update_partitions(
    const std::vector<Partition>& partitions
) {
    partitions_ = std::vector<Partition>(partitions.size(), Partition());
    value_to_partition_ = std::unordered_map<int, int>();

    for (auto i = 0; i < partitions.size(); i++) {
        const auto& partition = partitions.at(i);
        for (auto value: partition.data()) {
            auto value_weight = access_graph_.vertice_weight(value);
            partitions_[i].insert(value, value_weight);
            value_to_partition_.insert(std::make_pair(value, i));
        }
    }
}

int PartitionManager::allocate_value(int value) {
    if (value_to_partition_.find(value) != value_to_partition_.end()) {
        return value_to_partition_.at(value);
    }

    auto partition_id = round_robin_counter_;
    round_robin_counter_ = (round_robin_counter_ + 1) % partitions_.size();
    partitions_.at(partition_id).insert(value);
    value_to_partition_.insert(std::make_pair(value, partition_id));

    return partition_id;
}

void PartitionManager::add_value(int value, int partition, int n_accesses) {
    partitions_[partition].insert(value, n_accesses);
    value_to_partition_[value] = partition;
}

void PartitionManager::register_access(
    const std::unordered_set<int>& involved_values
) {
    update_graph(involved_values);
    update_partition(involved_values);
}

void PartitionManager::update_graph(
    const std::unordered_set<int>& involved_values
) {
    auto auxiliary_set = involved_values;
    for (auto value: involved_values) {
        auxiliary_set.erase(value);
        if (not access_graph_.exist_vertice(value)) {
            access_graph_.add_vertice(value);
        }
        access_graph_.increase_vertice_weight(value);

        for (auto joint_accessed_value: auxiliary_set) {
            if (not access_graph_.exist_vertice(joint_accessed_value)) {
                access_graph_.add_vertice(joint_accessed_value);
            }
            if (not access_graph_.are_connected(value, joint_accessed_value)) {
                access_graph_.add_edge(value, joint_accessed_value);
            }
            if (not access_graph_.are_connected(joint_accessed_value, value)) {
                access_graph_.add_edge(joint_accessed_value, value);
            }

            access_graph_.increase_edge_weight(value, joint_accessed_value);
            access_graph_.increase_edge_weight(joint_accessed_value, value);
        }
    }
}

void PartitionManager::update_partition(
    const std::unordered_set<int>& involved_values
) {
    for (auto value: involved_values) {
        if (value_to_partition_.find(value) == value_to_partition_.end()) {
            allocate_value(value);
        }
        auto partition_id = value_to_partition_.at(value);
        partitions_.at(partition_id).increase_weight(value, 1);
    }
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

const model::Graph& PartitionManager::access_graph() const {
    return access_graph_;
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
