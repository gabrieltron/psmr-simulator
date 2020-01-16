#include "min_cut_manager.h"

namespace workload {

MinCutManager::MinCutManager(
    int n_variables,
    int n_partitions,
    int repartition_interval,
    std::vector<long int> data_partition)
    :   Manager{n_variables},
        access_graph_{model::Graph(n_variables)},
        partition_scheme_{PartitionScheme(n_partitions, data_partition)}
{}

// Distribute data in partitions with round-robin
MinCutManager::MinCutManager(
    int n_variables,
    int n_partitions,
    int repartition_interval)
    :   Manager{n_variables},
        access_graph_{model::Graph(n_variables)},
        repartition_interval_{repartition_interval}
{
    auto data_partition = std::vector<long int>();
    auto current_partition = 0;
    for (auto i = 0; i < n_variables_; i++) {
        data_partition.push_back(current_partition);
        current_partition = (current_partition+1) % n_partitions;
    }
    partition_scheme_ = PartitionScheme(n_partitions, data_partition);
}

ExecutionLog MinCutManager::execute_requests() {
    auto log = ExecutionLog(partition_scheme_.n_partitions());

    while (!requests_.empty()) {
        auto request = requests_.front();
        requests_.pop_front();

        auto involved_partitions = std::unordered_set<int>();
        for (auto data : request) {
            involved_partitions.insert(
                partition_scheme_.data_partition(data)
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

        update_access_graph(request);
        log.increase_processed_requests();
        if (repartition_interval_ != 0 and
            log.processed_requests() % repartition_interval_ == 0
        ) {
            repartition_data(partition_scheme_.n_partitions());

            auto involved_partitions = std::unordered_set<int>();
            for (auto i = 0; i < partition_scheme_.n_partitions(); i++) {
                involved_partitions.insert(i);
            }
            log.sync_partitions(involved_partitions);
        }
    }

    return log;
}

void MinCutManager::update_access_graph(Request request) {
    for (auto first_data : request) {
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

void MinCutManager::repartition_data(int n_partitions) {
    auto data_partitions = model::cut_graph(access_graph_, n_partitions);
    partition_scheme_.update_partitions(n_partitions, data_partitions);
}

void MinCutManager::export_data(std::string output_path) {
    auto data_partitions = partition_scheme_.data_partition_vector();
    std::ofstream output_stream(output_path, std::ofstream::out);
    output::write_data_partitions(data_partitions, output_stream);
    output_stream.close();
}

model::Graph MinCutManager::access_graph() {
    return access_graph_;
}

PartitionScheme MinCutManager::partiton_scheme() {
    return partition_scheme_;
}

int MinCutManager::n_variables() {
    return n_variables_;
}

}
