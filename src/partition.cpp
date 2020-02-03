#include "partition.h"

namespace workload {

int Partition::weight() {
    return weight_; 
}

void Partition::insert(int vertice, int weight) {
    vertex_.insert(vertice);
    weight_ += weight;
}

std::unordered_set<int>& Partition::vertex() {
    return vertex_;
}

}