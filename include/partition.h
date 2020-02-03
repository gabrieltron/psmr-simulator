#ifndef MODEL_PARTITION_H
#define MODEL_PARTITION_H

#include <unordered_set>

namespace workload {

class Partition {
public:
    Partition() = default;

    void insert(int vertice, int weight);
    int weight();
    std::unordered_set<int>& vertex();

private:
    int weight_ = 0;
    std::unordered_set<int> vertex_;
};

}

#endif