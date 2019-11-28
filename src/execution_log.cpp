#include "execution_log.h"

namespace workload {

void ExecutionLog::increase_elapsed_time(int partition) {
    elapsed_time_[partition] += 1;
}

void ExecutionLog::skip_time(int partition, int value) {
    idle_time_[partition] += elapsed_time_[partition] - value;
    elapsed_time_[partition] = value;
}

void ExecutionLog::increase_sync_counter() {
    sync_counter_++;
}

int ExecutionLog::max_elapsed_time(std::unordered_set<int> request) {
    auto max = 0;
    for (auto data : request) {
        if (elapsed_time_[data] > max) {
            max = elapsed_time_[data];
        }
    }
    return max;
}


}