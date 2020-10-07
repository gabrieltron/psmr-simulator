#ifndef MODEL_PARTITION_H
#define MODEL_PARTITION_H

#include <unordered_map>
#include <unordered_set>

namespace workload {

class Partition {
public:
    Partition() = default;

    void insert(int data, int weight = 0);
    void remove(int data);
    void increase_weight(int data, int weight);
    int weight() const;
    const std::unordered_set<int>& data() const;

private:
    int total_weight_ = 0;
    std::unordered_set<int> data_set_;
    std::unordered_map<int, int> weight_;
};

}

#endif
