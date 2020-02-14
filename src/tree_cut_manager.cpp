#include "tree_cut_manager.h"

namespace workload {

TreeCutManager::TreeCutManager(
    int n_variables,
    int n_partitions,
    int repartition_interval,
    std::vector<idx_t> data_partition
    ) : MinCutManager{
            n_variables, n_partitions,
            repartition_interval, data_partition
        }
{}

// Distribute data in partitions with round-robin
TreeCutManager::TreeCutManager(
    int n_variables,
    int n_partitions,
    int repartition_interval
    ) : MinCutManager{
            n_variables, n_partitions,
            repartition_interval
        }
{}

void TreeCutManager::repartition_data(int n_partitions) {
    auto data_partitions = model::spanning_tree_cut(
        access_tree_, n_partitions
    );
    partition_scheme_ = data_partitions;
}

void TreeCutManager::update_access_structure(Request request) {
    for (auto first_data : request) {
        access_tree_.increase_node_weight(first_data, 1);
        for (auto second_data : request) {
            if (first_data == second_data) {
                continue;
            }

            auto edge = std::make_pair(first_data, second_data);
            if (access_tree_.is_detatched(first_data) or
                access_tree_.is_detatched(second_data))
            {
                access_tree_.add_edge(edge);
            }

            access_tree_.increase_edge_weight(edge, 1);
        }
    }
}

model::SpanningTree TreeCutManager::access_tree() {
    return access_tree_;
}

}