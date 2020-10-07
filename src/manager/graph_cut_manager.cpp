#include "graph_cut_manager.h"

namespace workload {

GraphCutManager::GraphCutManager(
    int n_variables,
    int n_partitions,
    int repartition_interval,
    std::vector<int> data_partition,
    model::CutMethod cut_method)
    :   MinCutManager{
            n_variables, n_partitions,
            repartition_interval, data_partition
        },
        access_graph_{model::Graph(n_variables)},
        cut_method_{cut_method}
{}

// Distribute data in partitions with round-robin
GraphCutManager::GraphCutManager(
    int n_variables,
    int n_partitions,
    int repartition_interval,
    model::CutMethod cut_method)
    :   MinCutManager{
            n_variables, n_partitions,
            repartition_interval
        },
        access_graph_{model::Graph(n_variables)},
        cut_method_{cut_method}
{}

void GraphCutManager::repartition_data(int n_partitions) {
    if (cut_method_ == model::FENNEL) {
        partition_manager_ = std::move(
            model::fennel_cut(access_graph_, partition_manager_.n_partitions())
        );
    } else if (cut_method_ == model::REFENNEL) {
        partition_manager_ = std::move(
            model::refennel_cut(access_graph_, partition_manager_)
        );
    } else {
        partition_manager_ = std::move(
            model::multilevel_cut(
                access_graph_, partition_manager_.n_partitions(), cut_method_
            )
        );
    }
}

void GraphCutManager::export_data(std::string output_path) {
    std::ofstream output_stream(output_path, std::ofstream::out);
    auto partitions_graph = partition_manager_.graph_representation();
    output::write_graph(partitions_graph, output::DOT, output_stream);
    output_stream.close();
}

void GraphCutManager::update_access_structure(const Request& request) {
    for (auto first_data : request) {
        if (not access_graph_.exist_vertice(first_data)) {
            access_graph_.add_vertice(first_data);
        }
        access_graph_.increase_vertice_weight(first_data);
        for (auto second_data : request) {
            if (first_data == second_data) {
                continue;
            }

            if (!access_graph_.are_connected(first_data, second_data)) {
                access_graph_.add_edge(first_data, second_data);
                access_graph_.add_edge(second_data, first_data);
            }

            access_graph_.increase_edge_weight(first_data, second_data);
        }
    }
}


model::Graph GraphCutManager::access_graph() {
    return access_graph_;
}

}
