#include "write.h"

namespace output {

void write_graph(model::Graph& graph, GraphFormats format, std::ostream& output_stream) {
    if (format == GraphFormats::METIS) {
        write_metis_format(graph, output_stream);
    } else if (format == GraphFormats::DOT) {
        write_dot_format(graph, output_stream);
    }
}

void write_metis_format(model::Graph& graph, std::ostream& output_stream) {
    output_stream << graph.n_vertex() << " " << graph.n_edges()/2 << " 011";
    for (auto i = 0; i < graph.n_vertex(); i++) {
        output_stream << "\n" << graph.vertice_weight(i);
        for (auto kv : graph.vertice_edges(i)) {
            auto neighbour = kv.first;
            auto edge_weight = kv.second;
            output_stream << " " << neighbour;
            output_stream << " " << edge_weight;
        }
    } 
}

void write_dot_format(model::Graph& graph, std::ostream& output_stream) {
    output_stream << "graph {\n";
    auto in_file = std::unordered_map<int, std::unordered_set<int>>();
    for (auto kv : graph.vertex()) {
        auto vertice = kv.first;
        auto weight = kv.second;
        in_file[vertice] = std::unordered_set<int>();

        std::stringstream id_stream;
        id_stream << "\"id:" << vertice << " w:" << weight << "\"";
        auto id = id_stream.str();
        output_stream << "    " << id << ";\n";

        for (auto kv_2 : graph.vertice_edges(vertice)) {
            auto neighbour = kv_2.first;
            auto edge_weight = kv_2.second;
            if (in_file[neighbour].find(vertice) != in_file[neighbour].end()) {
                continue;
            }

            std::stringstream neighbour_id_stream;
            neighbour_id_stream << "\"id:" << neighbour;
            neighbour_id_stream << " w:" << graph.vertice_weight(neighbour);
            neighbour_id_stream << "\"";
            auto neighbour_id = neighbour_id_stream.str();
            output_stream << "    " << id << " -- ";
            output_stream << neighbour_id;
            output_stream << "[label=\"" << edge_weight << "\"];\n";

            in_file[vertice].insert(neighbour);
        }
    }

    output_stream << "}";
}

void write_cut_info(
    model::Graph& graph,
    workload::PartitionScheme& partition_scheme,
    std::ostream& output_stream
) {
    auto weights = model::partitions_weight(graph, partition_scheme);
    output_stream << "Partição | Peso Vértices | Vértices\n"; 
    for (auto kv : partition_scheme.partitions()) {
        auto i = kv.first;
        auto data_set = kv.second;

        output_stream << i << " | " << weights[i]; 
        output_stream << " | ";
        for (auto data : data_set) {
            output_stream << " " << data; 
        }
        output_stream << "\n";
    }
}

void write_log_info(
    workload::ExecutionLog& execution_log,
    std::ostream& output_stream
) {
    auto makespan = execution_log.makespan();
    output_stream << "Makespan: " << makespan << "\n";
    output_stream << "Execution time:\n";
    auto execution_time = execution_log.execution_time();
    for (auto i = 0; i < execution_time.size(); i++) {
        output_stream << i << ": " << execution_time[i] << "\n";
    }

    output_stream << "Idle time:\n";
    auto idle_time = execution_log.idle_time();
    for (auto i = 0; i < idle_time.size(); i++) {
        output_stream << i << ": " << idle_time[i];
        auto idle_percentage = idle_time[i] / double(makespan) * 100.0;
        output_stream << " - " << idle_percentage << "%\n";
    }
    output_stream << "\n";

    output_stream << "Required syncs: " << execution_log.n_syncs() << "\n";
    output_stream << "Syncs involving partitions:\n";
    auto syncs_with_n_partitions = execution_log.syncs_with_n_partitions();
    for (auto i = 1; i <= syncs_with_n_partitions.size(); i++) {
        output_stream << i << " partitions: ";
        output_stream << syncs_with_n_partitions[i] << "\n";
    }
    output_stream << "\n";
}

void write_data_partitions(
    std::vector<long int> data_partitions,
    std::ostream& output_stream
) {
    output_stream << "data_partitions = [ ";
    for (auto partition : data_partitions) {
        output_stream << partition << ", ";
    }
    output_stream << "]";
}

}