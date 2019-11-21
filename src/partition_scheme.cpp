#include "partition_scheme.h"

namespace model {

PartitionScheme::PartitionScheme(
    Graph& og_graph,
    std::vector<long int>& vertex_partitions
) {
    for(auto vertice = 0; vertice < vertex_partitions.size(); vertice++) {
        auto partition = vertex_partitions[vertice];

        if (partitions_.find(partition) == partitions_.end()) {
            partitions_[partition] = Graph();
        }

        auto weight = og_graph.vertex_weight(vertice);
        partitions_[partition].add_vertice(vertice, weight);

        for (auto kv: og_graph.vertice_edges(vertice)) {
            auto neighbour = kv.first;
            auto edge_weight = kv.second;

            if (vertex_partitions[neighbour] != partition) {
                continue;
            }
                
            partitions_[partition].add_edge(
                vertice, neighbour, edge_weight
            );
        }
    }
}

void PartitionScheme::info(
    PartitionSchemeInfo& vertex_weight,
    PartitionSchemeInfo& edges_weight
) {
    for (auto kv: partitions_) {
        auto partition_id = kv.first;
        auto partition_graph = kv.second;

        vertex_weight[partition_id] = 0;
        edges_weight[partition_id] = 0;

        for (auto v_tuple: partition_graph.vertex()) {
            auto vertice = v_tuple.first;
            auto weight = v_tuple.second;

            vertex_weight[partition_id] += weight;

            for (auto e_tuple: partition_graph.vertice_edges(vertice)) {
                auto neighbour = e_tuple.first;
                auto edge_weight = e_tuple.second;

                if (vertice < neighbour) {
                    edges_weight[partition_id] +=edge_weight;
                }
            }
        } 
    }
}

void PartitionScheme::export_scheme(std::ostream& output_stream) {
    auto partitions_weight = PartitionSchemeInfo();
    auto partitions_edges_weight = PartitionSchemeInfo();
    info(partitions_weight, partitions_edges_weight);

    output_stream << "Partição | Peso Vértices | Peso Arestas | Vértices\n"; 
    for (auto kv : partitions_) {
        auto i = kv.first;

        output_stream << i << " | " << partitions_weight[i]; 
        output_stream << " | " << partitions_edges_weight[i] << " |";
        for (auto kv : partitions_[i].vertex()) {
            auto vertice = kv.first;
            output_stream << " " << vertice; 
        }
        output_stream << "\n";
    }
}

}