#ifndef MODEL_SPANNING_TREE_H
#define MODEL_SPANNING_TREE_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "graph.h"

namespace model {

typedef std::pair<int, int> Edge;

class SpanningTree : public Graph {
public:
    SpanningTree() = default;

    void add_edge(Edge edge, int weight = 1);
    void increase_node_weight(int node, int value);
    void increase_edge_weight(Edge edge, int value);
    void unpropagate_weight(int node);
    void print_stuff();

    int parent(int node);
    bool is_detatched(int id);
    const std::unordered_map<int, int>& id_to_node();
    const std::unordered_set<int>& ids_in_node(int node);

private:
    void add_vertice_with_neighbour(int new_vertice, int existing_node);

    std::unordered_set<int> detatched_ids_;
    std::unordered_map<int, int> parent_node_;
    std::unordered_map<int, int> id_to_node_;
    std::unordered_map<int, int> node_depth_;
    std::unordered_map<int, std::unordered_set<int>> ids_in_node_;
};

}

#endif