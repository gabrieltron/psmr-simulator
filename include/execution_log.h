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
    int max_elapsed_time(std::unordered_set<int> requests) const;

    int makespan() const;
    int n_threads() const;
    int n_syncs() const;
    int processed_requests() const;
    int elapsed_time(int partition) const;
    int idle_time() const;
    std::unordered_map<int, int> idle_time_per_partition() const;
    const std::unordered_map<int, int>& execution_time() const;
    const std::unordered_map<int, int>& crossborder_requests() const;

private:
    int sync_counter_ = 0;
    int processed_requests_ = 0;
    std::unordered_map<int, int> elapsed_time_;
    std::unordered_map<int, int> idle_time_;
    std::unordered_map<int, int> crossborder_requests_;
};

}

#endif
