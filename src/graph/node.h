#ifndef MODEL_NODE_H
#define MODEL_NODE_H

#include <utility>
#include <vector>

namespace model {

typedef std::pair<int, int> Parent;

class Node {
public:
    Node(int parent, int depth);

    void increase_weight(int value);
    void increase_edge_weight(int value);

    int weight();
    int parent_edge_weight();
    int depth();
    int parent_id();

private:
    int depth_, weight_{0};
    Parent parent_;
};

}

#endif