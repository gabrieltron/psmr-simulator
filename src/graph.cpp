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

std::size_t Graph::n_vertex() {
    return vertex_.size();
}

std::size_t Graph::n_edges() {
    return n_edges_;
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