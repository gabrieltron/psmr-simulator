#include "execution_log.h"

namespace workload {

ExecutionLog::ExecutionLog(int n_partitions) {
    for (auto i = 0; i < n_partitions; i++) {
        elapsed_time_[i] = 0;
        idle_time_[i] = 0;
        crossborder_requests_[i+1] = 0;
    }
}

void ExecutionLog::increase_elapsed_time(int partition) {
    elapsed_time_[partition] += 1;
}

void ExecutionLog::sync_partitions(std::unordered_set<int>& partitions) {
    auto timeskip = max_elapsed_time(partitions);
    for (auto partition : partitions) {
        skip_time(partition, timeskip);
    }
    crossborder_requests_[partitions.size()]++;
    if (partitions.size() > 1) {
        sync_counter_++;
    }
}

void ExecutionLog::skip_time(int partition, int value) {
    idle_time_[partition] += value - elapsed_time_[partition];
    elapsed_time_[partition] = value;
}

void ExecutionLog::increase_sync_counter() {
    sync_counter_++;
}

void ExecutionLog::increase_processed_requests() {
    processed_requests_++;
}

int ExecutionLog::max_elapsed_time(std::unordered_set<int> partitions) const {
    auto max = 0;
    for (auto partition : partitions) {
        if (elapsed_time_.at(partition) > max) {
            max = elapsed_time_.at(partition);
        }
    }
    return max;
}

int ExecutionLog::makespan() const {
    auto longest_time = 0;
    for (auto kv: elapsed_time_) {
        auto time = kv.second;
        if (longest_time < time) {
            longest_time = time;
        }
    }
    return longest_time;
}

int ExecutionLog::n_threads() const {
    return elapsed_time_.size();
}

int ExecutionLog::n_syncs() const {
    return sync_counter_;
}

int ExecutionLog::processed_requests() const {
    return processed_requests_;
}

int ExecutionLog::elapsed_time(int partition) const {
    return elapsed_time_.at(partition);
}

int ExecutionLog::idle_time() const {
    auto total_time = 0;
    for (auto kv : idle_time_) {
        auto partition = kv.first;
        auto time = kv.second;
        total_time += time + (makespan() - elapsed_time_.at(partition));
    }
    return total_time;
}

std::unordered_map<int, int> ExecutionLog::idle_time_per_partition() const {
    auto idle_time = idle_time_;

    for (auto kv: idle_time_) {
        auto partition = kv.first;
        idle_time[partition] += makespan() - elapsed_time_.at(partition);
    }

    return idle_time;
}

const std::unordered_map<int, int>& ExecutionLog::execution_time() const{
    return elapsed_time_;
}

const std::unordered_map<int, int>& ExecutionLog::crossborder_requests() const {
    return crossborder_requests_;
}

}
