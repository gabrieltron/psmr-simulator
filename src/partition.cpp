#include "partition.h"

namespace workload {

void Partition::increase_weight(int value) {
    weight_ += value;
}

int Partition::weight() {
    return weight_; 
}

void Partition::insert(int vertice, int weight /*= 0*/) {
    vertex_.insert(vertice);
    weight_ += weight;
}

const std::unordered_set<int>& Partition::vertex() {
    return vertex_;
}

}