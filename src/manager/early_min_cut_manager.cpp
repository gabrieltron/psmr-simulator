#include "early_min_cut_manager.h"

namespace workload {

EarlyMinCutManager::EarlyMinCutManager(
    int n_variables,
    int n_partitions,
    int repartition_window)
    :   Manager{n_variables},
        n_partitions_{n_partitions},
        repartition_window_{repartition_window}
{}

void EarlyMinCutManager::update_graph(
    model::Graph& graph, const Request& request
) const {
    for (auto first_data : request) {
        if(graph.exist_vertice(first_data)) {
            graph.increase_vertice_weight(first_data);
        } else {
            graph.add_vertice(first_data, 1);
        }

        for (auto second_data : request) {
            if (first_data == second_data) {
                continue;
            }

            if (!graph.are_connected(first_data, second_data)) {
                graph.add_edge(first_data, second_data);
                graph.add_edge(second_data, first_data);
            }

            graph.increase_edge_weight(first_data, second_data);
        }
    }
}

ExecutionLog EarlyMinCutManager::execute_requests() {
    auto log = ExecutionLog(n_partitions_);

    while (!requests_.empty()) {
        auto batch = std::vector<Request>();
        auto batch_graph = model::Graph();
        while (batch.size() != repartition_window_ and !requests_.empty()) {
            auto request = requests_.front();
            requests_.pop_front();
            batch.push_back(request);
            update_graph(batch_graph, request);
        }

        auto partition_scheme = model::fennel_cut(batch_graph, n_partitions_);

        for (auto& request : batch) {
            auto involved_partitions = std::unordered_set<int>();
            for (auto data : request) {
                involved_partitions.insert(
                    partition_scheme.value_to_partition(data)
                );
            }

            if (involved_partitions.size() == 1) {
                auto partition = *(involved_partitions.begin());
                log.increase_elapsed_time(partition);
            } else {
                log.sync_partitions(involved_partitions);
                for (auto partition : involved_partitions) {
                    log.increase_elapsed_time(partition);
                }
            }
        }
    }

    return log;
}

void EarlyMinCutManager::set_repartition_window(int repartition_window) {
    repartition_window_ = repartition_window;
}

void EarlyMinCutManager::set_n_partitions(int n_partitions) {
    n_partitions_ = n_partitions;
}

void EarlyMinCutManager::export_data(std::string output_path) {

}

}
