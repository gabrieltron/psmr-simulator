#include "graph.h"

namespace model {

Graph::Graph(int n_vertex)
    : n_edges_{0}
{
    for (auto i = 0; i < n_vertex; i++) {
        vertex_[i] = 0;
        edges_[i] = std::unordered_map<int, int>();
    }
}

void Graph::add_vertice(int id, int weight /*= 0*/) {
    vertex_[id] = weight;
}

void Graph::add_edge(int from, int to, int weight /*= 0*/) {
    if (edges_[from].find(to) == edges_[from].end()) {
        edges_[from][to] = 0;
    }

    edges_[from][to] = weight;
    n_edges_++;
}

void Graph::increase_vertice_weight(int vertice) {
    vertex_[vertice]++;
}

void Graph::increase_edge_weight(int from, int to) {
    edges_[from][to]++;
}

bool Graph::are_connected(int vertice_a, int vertice_b) {
    return edges_[vertice_a].find(vertice_b) != edges_[vertice_a].end();
}

void Graph::export_graph(std::ostream& output_stream, ExportFormat format) {
    if (format == ExportFormat::METIS) {
        export_metis(output_stream);
    } else if (format == ExportFormat::DOT) {
        export_dot(output_stream);
    }
}

void Graph::export_metis(std::ostream& output_stream) {
    output_stream << n_vertex() << " " << n_edges_ << " 011";
    for (auto i = 0; i < n_vertex(); i++) {
        output_stream << "\n" << vertex_[i];
        for (auto kv : edges_[i]) {
            auto neighbour = kv.first;
            auto edge_weight = kv.second;
            output_stream << " " << neighbour;
            output_stream << " " << edge_weight;
        }
    } 
}

void Graph::export_dot(std::ostream& output_stream) {
    output_stream << "graph {\n";

    auto in_file = std::unordered_map<int, std::unordered_set<int>>();
    for (auto kv : vertex_) {
        auto vertice = kv.first;
        auto weight = kv.second;
        in_file[vertice] = std::unordered_set<int>();

        std::stringstream id_stream;
        id_stream << "\"id:" << vertice << " w:" << weight << "\"";
        auto id = id_stream.str();
        output_stream << "    " << id << ";\n";

        for (auto kv_2 : edges_[vertice]) {
            auto neighbour = kv_2.first;
            auto edge_weight = kv_2.second;
            if (in_file[neighbour].find(vertice) != in_file[neighbour].end()) {
                continue;
            }

            std::stringstream neighbour_id_stream;
            neighbour_id_stream << "\"id:" << neighbour;
            neighbour_id_stream << " w:" << vertex_[neighbour] << "\"";
            auto neighbour_id = neighbour_id_stream.str();
            output_stream << "    " << id << " -- ";
            output_stream << neighbour_id;
            output_stream << "[label=\"" << edge_weight << "\"];\n";

            in_file[vertice].insert(neighbour);
        }
    }

    output_stream << "}";
}

std::size_t Graph::n_vertex() {
    return vertex_.size();
}

int Graph::vertice_weight(int vertice) {
    return vertex_[vertice];
}

std::unordered_map<int, int> Graph::vertice_edges(int vertice) {
    return edges_[vertice];
}

Vertex Graph::vertex() {
    return vertex_;
}

}