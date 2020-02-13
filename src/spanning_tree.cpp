#include "spanning_tree.h"

namespace model {

void SpanningTree::increase_node_weight(int node, int value) {
    auto current_node = node;
    while (current_node != -1) {
        increase_vertice_weight(current_node, value);
        current_node = parent_node_[current_node];
    }
}

void SpanningTree::increase_edge_weight(Edge edge, int value) {
    auto u_id = id_to_node_[edge.first];
    auto v_id = id_to_node_[edge.second];

    while (node_depth_[u_id] > node_depth_[v_id]) {
        auto parent = parent_node_[u_id];
        Graph::increase_edge_weight(u_id, parent, value);
        u_id = parent_node_[u_id];
    }
    while (node_depth_[u_id] < node_depth_[v_id]) {
        auto parent = parent_node_[v_id];
        Graph::increase_edge_weight(v_id, parent, value);
        v_id = parent_node_[v_id];
    }

    while (v_id != u_id) {
        auto u_parent = parent_node_[u_id];
        Graph::increase_edge_weight(u_id, u_parent, value);
        u_id = parent_node_[u_id];

        auto v_parent = parent_node_[v_id];
        Graph::increase_edge_weight(v_id, v_parent, value);
        v_id = parent_node_[v_id];
    }
}

void SpanningTree::add_vertice_with_neighbour(int new_id, int existing_node) {
    if (vertex_.find(existing_node) == vertex_.end()) {
        auto parent = id_to_node_[existing_node];
        auto parent_depth = node_depth_[parent];

        parent_node_[existing_node] = parent;
        node_depth_[existing_node] = parent_depth + 1;
    }
    id_to_node_[new_id] = existing_node;
    ids_in_node_[existing_node].insert(new_id);
    increase_node_weight(existing_node, 1);
}

void SpanningTree::add_edge(Edge edge, int weight /*= 1*/) {
    auto u = edge.first;
    auto v = edge.second;
    if (id_to_node_.find(u) == id_to_node_.end() and
        id_to_node_.find(v) == id_to_node_.end()
    ) {
        parent_node_[u] = -1;
        node_depth_[u] = 1;

        id_to_node_[u] = u;
        id_to_node_[v] = u;
        ids_in_node_[u].insert(u);
        ids_in_node_[u].insert(v);
        increase_node_weight(u, 2);
    } else {
        if (id_to_node_.find(u) == id_to_node_.end()) {
            add_vertice_with_neighbour(u, v);
            detatched_ids_.erase(u);
        } else if (id_to_node_.find(v) == id_to_node_.end()) {
            add_vertice_with_neighbour(v, u);
            detatched_ids_.erase(v);
        }
        
        increase_edge_weight(edge, weight);
    }
}

void SpanningTree::unpropagate_weight(int node) {
    auto weight = vertex_[node];
    auto current_node = parent_node_[node];
    while (current_node != -1) {
        increase_vertice_weight(node, -weight);
        current_node = parent_node_[current_node];
    }
}

const std::unordered_map<int, int>&  SpanningTree::id_to_node() {
    return id_to_node_;
}

const std::unordered_set<int>& SpanningTree::ids_in_node(int node) {
    return ids_in_node_[node];
}

bool SpanningTree::is_detatched(int id) {
    return detatched_ids_.find(id) != detatched_ids_.end();
}

int SpanningTree::parent(int node) {
    return parent_node_[node];
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
        if (vertex_.find(i) == vertex_.end() or detatched_ids_.find(i) != detatched_ids_.end()) {
            continue;
        }

        std::cout << "Supernode " << p[i] << " has weight " << vertex_[i] << std::endl;
        std::cout << " the edge that connects it to " << p[parent_node_[i]] << " has weight " << edges_[i][parent_node_[i]] << std::endl;
    }
}


}