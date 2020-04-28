#include "graph.h"

namespace model {

Graph::Graph(int n_vertex)
    : n_edges_{0}
{
    for (auto i = 0; i < n_vertex; i++) {
        vertex_[i] = 0;
        edges_[i] = std::unordered_map<int, int>();
        in_degree_[i] = 0;
    }
}

void Graph::add_vertice(int id, int weight /*= 0*/) {
    vertex_[id] = weight;
    in_degree_[id] = 0;
    total_vertex_weight_ += weight;
}

void Graph::remove_vertice(int vertice_id) {
    total_vertex_weight_ -= vertex_[vertice_id];
    vertex_.erase(vertice_id);
    edges_.erase(vertice_id);
    in_degree_.erase(vertice_id);
}

void Graph::add_edge(int from, int to, int weight /*= 0*/) {
    if (edges_[from].find(to) == edges_[from].end()) {
        edges_[from][to] = 0;
    }

    edges_[from][to] = weight;
    in_degree_[to]++;
    n_edges_++;
    total_edges_weight_ += weight;
}

void Graph::remove_edge(int from, int to) {
    if (edges_[from].find(to) != edges_[from].end()) {
        total_edges_weight_ -= edges_[from][to];
        edges_[from].erase(to);
        in_degree_[to]--;
    }
}

void Graph::increase_vertice_weight(int vertice, int value /*=1*/) {
    total_vertex_weight_ += value;
    vertex_[vertice] += value;
}

void Graph::increase_edge_weight(int from, int to, int value /*=1*/) {
    edges_[from][to] += value;
    total_edges_weight_ += value;
}

void Graph::set_vertice_weight(int vertice, int weight) {
    total_vertex_weight_ -= vertex_[vertice];
    vertex_[vertice] = weight;
    total_edges_weight_ += weight;
}

bool Graph::exist_vertice(int vertice) const {
    return vertex_.find(vertice) != vertex_.end();
}

bool Graph::are_connected(int vertice_a, int vertice_b) const {
    return edges_.at(vertice_a).find(vertice_b) != edges_.at(vertice_a).end();
}

std::size_t Graph::n_vertex() const {
    return vertex_.size();
}

std::size_t Graph::n_edges() const {
    return n_edges_;
}

int Graph::vertice_weight(int vertice) const {
    return vertex_.at(vertice);
}

int Graph::edge_weight(int from, int to) const {
    return edges_.at(from).at(to);
}

int Graph::total_vertex_weight() const {
    return total_vertex_weight_;
}

int Graph::total_edges_weight() const {
    return total_edges_weight_;
}

const std::unordered_map<int, int>& Graph::vertice_edges(int vertice) const {
    return edges_.at(vertice);
}

const Vertex& Graph::vertex() const {
    return vertex_;
}

int Graph::in_degree(int vertice) const {
    return in_degree_.at(vertice);
}

const std::unordered_map<int, int>& Graph::in_degrees() const {
    return in_degree_;
}

}
