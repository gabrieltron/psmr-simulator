#include "min_cut_manager.h"

namespace workload {

MinCutManager::MinCutManager(
    int n_variables,
    int n_partitions,
    int repartition_interval,
    std::vector<int> data_partition)
    :   Manager{n_variables},
        partition_manager_{PartitionManager(n_partitions, data_partition)}
{}

// Distribute data in partitions with round-robin
MinCutManager::MinCutManager(
    int n_variables,
    int n_partitions,
    int repartition_interval)
    :   Manager{n_variables},
        repartition_interval_{repartition_interval}
{
    std::vector<int> values;
    for (auto i = 0; i < n_variables_; i++) {
        values.push_back(i);
    }
    partition_manager_ = PartitionManager(n_partitions, values);
}

ExecutionLog MinCutManager::execute_requests() {
    auto log = ExecutionLog(partition_manager_.n_partitions());

    while (!requests_.empty()) {
        auto request = requests_.front();
        requests_.pop_front();

        auto involved_partitions = get_involved_partitions(request);

        auto executing_partition = *involved_partitions.begin();
        log.execute_request(executing_partition);
        if (involved_partitions.size() > 1) {
            log.sync_partitions(involved_partitions);
        }

        partition_manager_.register_access(request);
        log.increase_processed_requests();

        bool should_repartition = repartition_interval_ != 0 and
            log.processed_requests() % repartition_interval_ == 0;
        if (should_repartition) {
            repartition_data(partition_manager_.n_partitions());
            log.sync_all_partitions();
        }
    }

    return log;
}

std::unordered_set<int> MinCutManager::get_involved_partitions(
    const Request& request
) {
    std::unordered_set<int> involved_partitions;
    for (auto value: request) {
	    if (not partition_manager_.in_scheme(value)) {
            partition_manager_.allocate_value(value);
        }
        auto partition_id = partition_manager_.value_to_partition(value);
        involved_partitions.insert(
            partition_manager_.value_to_partition(partition_id)
        );
    }
    return involved_partitions;
}


void MinCutManager::export_data(std::string output_path) {
    auto data_partitions = partition_manager_.value_to_partition_map();
    std::ofstream output_stream(output_path, std::ofstream::out);
    output::write_data_partitions(data_partitions, output_stream);
    output_stream.close();
}

PartitionManager MinCutManager::partition_manager() {
    return partition_manager_;
}

}
