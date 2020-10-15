#ifndef WORKLOAD_TREE_CUT_MANAGER_H
#define WORKLOAD_TREE_CUT_MANAGER_H

#include <vector>

#include "partition/min_cut.h"
#include "graph/spanning_tree.h"
#include "min_cut_manager.hpp"

namespace workload {

class TreeCutManager : public MinCutManager {
public:
    TreeCutManager() = default;
    TreeCutManager(
        int n_variables,
        int n_partitions,
        int repartition_interval,
        std::vector<int> data_partition
    );
    TreeCutManager(
        int n_variables,
        int n_partitions,
        int repartition_interval
    );

    void initialize_tree();
    const model::SpanningTree& access_tree();
    void repartition_data(int n_partitions);
    void export_data(std::string output_path);

private:
    void update_access_structure(const Request& request);

    model::SpanningTree access_tree_;

};

}

#endif
