#ifndef MODEL_PARTITION_SCHEME_H
#define MODEL_PARTITION_SCHEME_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph/graph.h"
#include "partition.h"

namespace workload{

class PartitionManager {
public:
    PartitionManager() = default;
    PartitionManager(int n_partitions, const std::vector<int>& values);
    PartitionManager(
        std::vector<Partition>& partitions
    );

    int allocate_value(int value);
    void add_value(int value, int partition, int n_accesses);
    void register_access(const std::unordered_set<int>& involved_values);
    void increase_partition_weight(int partition_id, int weight=1);
    void remove_value(int value);
    void update_partitions(std::vector<Partition>& partitions);
    bool in_scheme(int value) const;

    int n_partitions() const;
    int value_to_partition(int value) const;
    const std::vector<Partition>& partitions() const;
    const std::unordered_map<int, int>& value_to_partition_map() const;
    model::Graph graph_representation() const;

private:
    int round_robin_counter_;

    void update_graph(const std::unordered_set<int>& involved_values);
    void update_partition(const std::unordered_set<int>& involved_values);

    model::Graph access_graph_;
    std::unordered_map<int, int> value_to_partition_;
    std::vector<Partition> partitions_;
};

}

#endif
