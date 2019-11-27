#include "graph_cut.h"

namespace model {

std::vector<long int> cut_graph(Graph& graph, idx_t n_partitions) {
    auto vertex = graph.vertex();
    idx_t n_vertice = vertex.size();
    idx_t n_edges = n_vertice * (n_vertice - 1);
    idx_t n_constrains = 1;

    auto vertice_weight = std::vector<idx_t>();
    for (auto i = 0; i < vertex.size(); i++) {
        vertice_weight.push_back(vertex[i]);
    }

    auto x_edges = std::vector<idx_t>();
    auto edges = std::vector<idx_t>();
    auto edges_weight = std::vector<idx_t>();

    x_edges.push_back(0);
    for (auto vertice = 0; vertice < vertex.size(); vertice++) {
        auto last_edge_index = x_edges.back();
        auto n_neighbours = graph.vertice_edges(vertice).size();
        x_edges.push_back(last_edge_index + n_neighbours);

        for (auto vk: graph.vertice_edges(vertice)) {
            auto neighbour = vk.first;
            auto weight = vk.second;
            edges.push_back(neighbour);
            edges_weight.push_back(weight);
        }
    }

    idx_t options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);
    options[METIS_OPTION_OBJTYPE] = METIS_OBJTYPE_CUT;
    options[METIS_OPTION_NUMBERING] = 0;
    //options[METIS_OPTION_DBGLVL] = 511;

    idx_t objval;
    auto vertex_partitions = std::vector<idx_t>(n_vertice, 0);
    METIS_PartGraphKway(
        &n_vertice, &n_constrains, x_edges.data(), edges.data(),
        vertice_weight.data(), NULL, edges_weight.data(), &n_partitions, NULL,
        NULL, options, &objval, vertex_partitions.data()
    );
    
    return vertex_partitions;
}

std::unordered_map<int, int> partitions_weight(
    Graph& graph, workload::PartitionScheme& partition_scheme
) {
    auto partition_weight = std::unordered_map<int, int>();
    auto partitions = partition_scheme.partitions();
    for (auto kv: partitions) {
        auto partition_id = kv.first;
        auto partition_vertex = kv.second;

        partition_weight[partition_id] = 0;

        for (auto vertice: partition_vertex) {
            auto weight = graph.vertice_weight(vertice);
            partition_weight[partition_id] += weight;
        } 
    }

    return partition_weight;
}

void export_partitons_weight(
    Graph& graph,
    workload::PartitionScheme& partition_scheme,
    std::ostream& output_stream
) {
    auto partition_weight = partitions_weight(graph, partition_scheme);

    output_stream << "Partição | Peso Vértices | Vértices\n"; 
    for (auto kv : partition_scheme.partitions()) {
        auto i = kv.first;
        auto data_set = kv.second;

        output_stream << i << " | " << partition_weight[i]; 
        output_stream << " | ";
        for (auto data : data_set) {
            output_stream << " " << data; 
        }
        output_stream << "\n";
    }
}

}