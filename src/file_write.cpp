#include "file_write.h"

namespace output {

void write_graph(model::Graph& graph, GraphFormats format, std::string path) {
    if (format == GraphFormats::METIS) {
        write_metis_format(graph, path);
    } else if (format == GraphFormats::DOT) {
        write_dot_format(graph, path);
    }
}

void write_metis_format(model::Graph& graph, std::string path) {
    std::ofstream output_stream(path, std::ofstream::out);

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

    output_stream.close();
}

void write_dot_format(model::Graph& graph, std::string path) {
    std::ofstream output_stream(path, std::ofstream::out);

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
    output_stream.close();
}

}