#ifndef WORKLOAD_EXECUTION_LOG_H
#define WORKLOAD_EXECUTION_LOG_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>

namespace workload {

class ExecutionLog {
public:
    ExecutionLog(int n_data);

    void increase_elapsed_time(int partition);
    void sync_partitions(std::unordered_set<int>& partitions);
    void skip_time(int partition, int value);
    void increase_sync_counter();
    void increase_processed_requests();
    int max_elapsed_time(std::unordered_set<int> requests);

    int makespan();
    int n_syncs();
    int processed_requests();
    int elapsed_time(int partition);
    std::unordered_map<int, int> idle_time();
    std::unordered_map<int, int> execution_time();
    std::unordered_map<int, int> syncs_with_n_partitions();

private:
    int sync_counter_ = 0;
    int processed_requests_ = 0;
    std::unordered_map<int, int> elapsed_time_;
    std::unordered_map<int, int> idle_time_;
    std::unordered_map<int, int> syncs_with_n_partitions_;
};

}

#endif
