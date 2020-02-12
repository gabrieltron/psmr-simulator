#include "spanning_tree.h"

namespace model {

void SpanningTree::increase_node_weight(int node, int value) {
    auto current_node = node;
    while (current_node != -1) {
        nodes_[current_node]->increase_weight(value);
        current_node = nodes_[current_node]->parent_id();
    }
}

void SpanningTree::increase_edge_weight(Edge edge, int value) {
    auto u_id = vertice_to_id_[edge.first];
    auto v_id = vertice_to_id_[edge.second];

    while (nodes_[u_id]->depth() > nodes_[v_id]->depth()) {
        nodes_[u_id]->increase_edge_weight(value);
        u_id = nodes_[u_id]->parent_id();
    }
    while (nodes_[u_id]->depth() < nodes_[v_id]->depth()) {
        nodes_[v_id]->increase_edge_weight(value);
        v_id = nodes_[v_id]->parent_id();
    }

    while (v_id != u_id) {
        nodes_[u_id]->increase_edge_weight(value);
        u_id = nodes_[u_id]->parent_id();

        nodes_[v_id]->increase_edge_weight(value);
        v_id = nodes_[v_id]->parent_id();
    }
}

void SpanningTree::add_vertice_with_neighbour(int new_vertice, int existing_node) {
    if (nodes_.find(existing_node) != nodes_.end()) {
        vertice_to_id_[new_vertice] = existing_node;
    } else {
        auto parent_id = vertice_to_id_[existing_node];
        auto parent_depth = nodes_[parent_id]->depth();
        nodes_[existing_node] = std::make_unique<Node>(parent_id, parent_depth + 1);
        vertice_to_id_[new_vertice] = existing_node;
    }
    increase_node_weight(existing_node, 1);
}

void SpanningTree::add_edge(Edge edge, int weight /*= 1*/) {
    auto u = edge.first;
    auto v = edge.second;
    if (vertice_to_id_.find(u) == vertice_to_id_.end() and
        vertice_to_id_.find(v) == vertice_to_id_.end()
    ) {
        nodes_[u] = std::make_unique<Node>(-1, 1);
        vertice_to_id_[u] = u;
        vertice_to_id_[v] = u;
        increase_node_weight(u, 2);
    } else {
        if (vertice_to_id_.find(u) == vertice_to_id_.end()) {
            add_vertice_with_neighbour(u, v);
        } else if (vertice_to_id_.find(v) == vertice_to_id_.end()) {
            add_vertice_with_neighbour(v, u);
        }
        
        increase_edge_weight(edge, weight);
    }
}

const std::unordered_map<int, int>&  SpanningTree::vertice_to_id() {
    return vertice_to_id_;
}

void SpanningTree::print_stuff() {
    std::unordered_map<int, char> p({
        {0, 'a'},
        {1, 'b'},
        {2, 'c'},
        {3, 'd'},
        {4, 'e'},
        {5, 'f'},
        {6, 'g'},
        {7, 'h'},

    });
    for (auto i = 0; i < 8; i++) {
        if (nodes_.find(i) == nodes_.end()) {
            continue;
        }

        std::cout << "Supernode " << p[i] << " has weight " << nodes_[i]->weight() << std::endl;
        std::cout << " the edge that connects it to " << p[nodes_[i]->parent_id()] << " has weight " << nodes_[i]->parent_edge_weight() << std::endl;
    }
}


}