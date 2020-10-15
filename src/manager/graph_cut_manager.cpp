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
        cut_method_{cut_method}
{}

void GraphCutManager::set_cut_method(model::CutMethod cut_method) {
    cut_method_ = cut_method;
}

void GraphCutManager::repartition_data(int n_partitions) {
    if (cut_method_ == model::FENNEL) {
        partition_manager_.update_partitions(
            model::fennel_cut(
                partition_manager_.access_graph(),
                partition_manager_.n_partitions()
            )
        );
    } else if (cut_method_ == model::REFENNEL) {
        partition_manager_.update_partitions(
            model::refennel_cut(partition_manager_)
        );
    } else {
        partition_manager_.update_partitions(
            model::multilevel_cut(
                partition_manager_.access_graph(),
                partition_manager_.n_partitions(),
                cut_method_
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

}
