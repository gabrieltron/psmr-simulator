#ifndef MODEL_PARTITION_SCHEME_H
#define MODEL_PARTITION_SCHEME_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.h"

namespace workload{

typedef std::unordered_map<int, std::unordered_set<int>> Partitions;

class PartitionScheme {
public:
    PartitionScheme(
        std::vector<long int>& data_partitions
    );

    void update_partitions(const std::vector<long int>& data_partitions);
    model::Graph graph_representation();
    Partitions partitions();

    void export_as_graph(std::ostream& output_stream);

private:
    std::vector<long int> data_partitions_;
    Partitions partitions_;
};

}

#endif