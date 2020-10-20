#ifndef WORKLOAD_EXECUTION_LOG_H
#define WORKLOAD_EXECUTION_LOG_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>

namespace workload {

class ExecutionLog {
public:
    ExecutionLog(int n_threads);

    void increase_elapsed_time(int thread_id, int time=1);
    void execute_request(int thread_id, int execution_time=1);
    void sync_all_partitions();
    void sync_partitions(const std::unordered_set<int>& thread_ids);
    void skip_time(int thread, int value);
    void increase_sync_counter();
    int max_elapsed_time(const std::unordered_set<int>& thread_ids) const;

    int makespan() const;
    int n_threads() const;
    int n_syncs() const;
    int processed_requests() const;
    int elapsed_time(int thread_id) const;
    int idle_time() const;
    std::unordered_map<int, int> idle_time_per_thread() const;
    std::unordered_map<int, int> requests_per_thread() const;
    const std::unordered_map<int, int> execution_time() const;
    const std::unordered_map<int, int>& crossborder_requests() const;

private:
    int sync_counter_ = 0;
    int processed_requests_ = 0;
    std::unordered_map<int, int> crossborder_requests_;

    struct Thread {
        int requests_exectued_ = 0;
        int elapsed_time_ = 0;
        int idle_time_ = 0;
        int executed_requests_ = 0;
    };
    std::unordered_map<int, Thread> simulated_threads_;
};

}

#endif
