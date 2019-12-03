#include "execution_log.h"

namespace workload {

ExecutionLog::ExecutionLog(int n_partitions) {
    for (auto i = 0; i < n_partitions; i++) {
        elapsed_time_[i] = 0;
        idle_time_[i] = 0;
        syncs_with_n_partitions_[i+1] = 0;
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
    syncs_with_n_partitions_[partitions.size()]++;
    sync_counter_++;
}

void ExecutionLog::skip_time(int partition, int value) {
    idle_time_[partition] += value - elapsed_time_[partition];
    elapsed_time_[partition] = value;
}

void ExecutionLog::increase_sync_counter() {
    sync_counter_++;
}

int ExecutionLog::max_elapsed_time(std::unordered_set<int> partitions) {
    auto max = 0;
    for (auto partition : partitions) {
        if (elapsed_time_[partition] > max) {
            max = elapsed_time_[partition];
        }
    }
    return max;
}

int ExecutionLog::makespan() {
    auto longest_time = 0;
    for (auto kv: elapsed_time_) {
        auto time = kv.second;
        if (longest_time < time) {
            longest_time = time;
        }
    }
    return longest_time;
}

int ExecutionLog::n_syncs() {
    return sync_counter_;
}

std::unordered_map<int, int> ExecutionLog::idle_time() {
    auto idle_time = idle_time_;

    for (auto kv: idle_time_) {
        auto partition = kv.first;
        idle_time[partition] += makespan() - elapsed_time_[partition];
    }

    return idle_time;
}

std::unordered_map<int, int> ExecutionLog::execution_time() {
    return elapsed_time_;
}

std::unordered_map<int, int> ExecutionLog::syncs_with_n_partitions() {
    return syncs_with_n_partitions_;
}

}