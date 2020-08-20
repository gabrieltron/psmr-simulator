#include "min_cut_manager.h"

namespace workload {

MinCutManager::MinCutManager(
    int n_variables,
    int n_partitions,
    int repartition_interval,
    std::vector<int> data_partition)
    :   Manager{n_variables},
        partition_scheme_{PartitionScheme(n_partitions, data_partition)}
{}

// Distribute data in partitions with round-robin
MinCutManager::MinCutManager(
    int n_variables,
    int n_partitions,
    int repartition_interval)
    :   Manager{n_variables},
        repartition_interval_{repartition_interval}
{
    auto data_partition = std::vector<int>();
    for (auto i = 0; i < n_variables_; i++) {
        data_partition.push_back(round_robin_counter_);
        round_robin_counter_ = (round_robin_counter_+1) % n_partitions;
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
            if (not partition_scheme_.in_scheme(data)) {
                partition_scheme_.add_data(data, round_robin_counter_, 0);
                round_robin_counter_ =
                    (round_robin_counter_+1) % partition_scheme_.n_partitions();
            }
            involved_partitions.insert(
                partition_scheme_.data_partition(data)
            );
        }

        auto executing_partition = *involved_partitions.begin();
        log.execute_request(executing_partition);
        if (involved_partitions.size() > 1) {
            log.sync_partitions(involved_partitions);
        }

        update_access_structure(request);
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

void MinCutManager::export_data(std::string output_path) {
    auto data_partitions = partition_scheme_.data_partition_map();
    std::ofstream output_stream(output_path, std::ofstream::out);
    output::write_data_partitions(data_partitions, output_stream);
    output_stream.close();
}

PartitionScheme MinCutManager::partiton_scheme() {
    return partition_scheme_;
}

}
