#ifndef MODEL_SPANNING_TREE_H
#define MODEL_SPANNING_TREE_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>

#include "node.h"

namespace model {

typedef std::pair<int, int> Edge;

class SpanningTree {
public:
    SpanningTree() = default;

    void add_edge(Edge edge, int weight = 1);
    void increase_node_weight(int node, int value);
    void increase_edge_weight(Edge edge, int value);
    void print_stuff();

    const std::unordered_map<int, int>& vertice_to_id();

private:
    void add_vertice_with_neighbour(int new_vertice, int existing_node);

    std::unordered_map<int, std::unique_ptr<Node>> nodes_;
    std::unordered_map<int, int> vertice_to_id_;
};

}

#endif