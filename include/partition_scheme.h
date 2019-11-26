#ifndef MODEL_PARTITION_SCHEME_H
#define MODEL_PARTITION_SCHEME_H

#include <unordered_map>
#include <vector>

#include "graph.h"

namespace model{

typedef std::unordered_map<int, int> PartitionSchemeInfo;

class PartitionScheme {
public:
    PartitionScheme(
        model::Graph& og_graph,
        std::vector<long int>& vertex_partitions
    );

    void info(
        PartitionSchemeInfo& vertex_weight,
        PartitionSchemeInfo& edges_weight
    );

    void export_info(std::ostream& output_stream);
    void export_graph(std::ostream& output_stream);

private:
    std::unordered_map<int, Graph> partitions_;
};

}

#endif