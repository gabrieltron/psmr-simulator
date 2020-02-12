#include "node.h"

namespace model {

Node::Node(int parent, int depth)
    : depth_{depth}
{
    parent_ = std::make_pair(parent, 0);
}

void Node::increase_weight(int value) {
    weight_ += value;
}

void Node::increase_edge_weight(int value) {
    parent_.second += value;
}

int Node::weight() {
    return weight_;
}

int Node::parent_edge_weight() {
    return parent_.second;
}

int Node::depth() {
    return depth_;
}

int Node::parent_id() {
    return parent_.first;
}

}