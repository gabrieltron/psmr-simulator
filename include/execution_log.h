#ifndef WORKLOAD_EXECUTION_LOG_H
#define WORKLOAD_EXECUTION_LOG_H

#include <unordered_map>
#include <unordered_set>

namespace workload {

class ExecutionLog {
public:
    ExecutionLog() = default;
    
    void increase_elapsed_time(int partition);
    void skip_time(int partition, int value);
    void increase_sync_counter();
    int max_elapsed_time(std::unordered_set<int> request);

private:
    int sync_counter_ = 0;
    std::unordered_map<int, int> elapsed_time_;
    std::unordered_map<int, int> idle_time_;
};

}

#endif