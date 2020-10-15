#include "tree_cut_manager.h"

namespace workload {

TreeCutManager::TreeCutManager(
    int n_variables,
    int n_partitions,
    int repartition_interval,
    std::vector<int> data_partition
    ) : MinCutManager{
            n_variables, n_partitions,
            repartition_interval, data_partition
        },
        access_tree_{
            std::move(model::SpanningTree(n_variables))
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
        },
        access_tree_{
            std::move(model::SpanningTree(n_variables))
        }
{}

void TreeCutManager::repartition_data(int n_partitions) {
    auto data_partitions = model::spanning_tree_cut(
        access_tree_, n_partitions
    );
    partition_manager_.update_partitions(data_partitions);
}

void TreeCutManager::update_access_structure(const Request& request) {
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
            } else {
                access_tree_.increase_edge_weight(edge, 1);
            }
        }
    }
}

void TreeCutManager::initialize_tree() {
    access_tree_ = model::SpanningTree(n_variables_);
}

const model::SpanningTree& TreeCutManager::access_tree() {
    return access_tree_;
}

void TreeCutManager::export_data(std::string output_path) {
    auto data_partitions = partition_manager_.value_to_partition_map();
    std::ofstream output_stream(output_path, std::ofstream::out);
    output::write_data_partitions(data_partitions, output_stream);
    output_stream << "\n\n";
    output::write_spanning_tree(access_tree_, output_stream);
}

}
