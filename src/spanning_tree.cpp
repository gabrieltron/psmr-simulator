#include "spanning_tree.h"

namespace model {

SpanningTree::SpanningTree(int n_ids)
    : Graph{n_ids}
{
    for (auto i = 0; i < n_ids; i++) {
        detatched_ids_.insert(i);
    }
}

void SpanningTree::increase_node_weight(int node, int value /*=1*/) {
    if (detatched_ids_.find(node) != detatched_ids_.end()) {
        Graph::increase_vertice_weight(node, value);
    } else {
        auto current_node = id_to_node_[node];
        while (current_node != -1) {
            Graph::increase_vertice_weight(current_node, value);
            current_node = parent_node_[current_node];
        }
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
    if (parent_node_.find(existing_node) == parent_node_.end()) {
        auto parent = id_to_node_[existing_node];
        auto parent_depth = node_depth_[parent];

        parent_node_[existing_node] = parent;
        node_depth_[existing_node] = parent_depth + 1;
    }
    id_to_node_[new_id] = existing_node;
    ids_in_nodes_[existing_node].insert(new_id);
    increase_node_weight(existing_node, vertice_weight(new_id));
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
        ids_in_nodes_[u].insert(u);
        ids_in_nodes_[u].insert(v);
        auto node_weight = vertice_weight(u) + vertice_weight(v);
        increase_node_weight(u, node_weight);
    } else {
        if (id_to_node_.find(u) == id_to_node_.end()) {
            add_vertice_with_neighbour(u, v);
        } else if (id_to_node_.find(v) == id_to_node_.end()) {
            add_vertice_with_neighbour(v, u);
        }
        
        increase_edge_weight(edge, weight);
    }
    
    detatched_ids_.erase(v);
    detatched_ids_.erase(u);
}

void SpanningTree::unpropagate_weight(int node) {
    auto weight = vertex_[node];
    auto current_node = id_to_node_[node];
    while (current_node != -1) {
        increase_vertice_weight(node, -weight);
        current_node = parent_node_[current_node];
    }
}

const std::unordered_map<int, int>& SpanningTree::id_to_node() const {
    return id_to_node_;
}

const std::unordered_map<int, std::unordered_set<int>>& SpanningTree::ids_in_nodes() const {
    return ids_in_nodes_;
}

const std::unordered_set<int>& SpanningTree::ids_in_node(int node) const {
    return ids_in_nodes_.at(node);
}

bool SpanningTree::is_detatched(int id) const {
    return detatched_ids_.find(id) != detatched_ids_.end();
}

bool SpanningTree::is_inside_node(int id) const {
    return parent_node_.find(id) == parent_node_.end();
};

int SpanningTree::parent(int node) const {
    return parent_node_.at(node);
}

}