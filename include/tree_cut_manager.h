#ifndef WORKLOAD_TREE_CUT_MANAGER_H
#define WORKLOAD_TREE_CUT_MANAGER_H

#include <vector>

#include "min_cut.h"
#include "min_cut_manager.h"
#include "spanning_tree.h"

namespace workload {

class TreeCutManager : public MinCutManager {
public:
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

    const model::SpanningTree& access_tree();
    void repartition_data(int n_partitions);
    void export_data(std::string output_path);

private:
    void update_access_structure(Request request);

    model::SpanningTree access_tree_;

};

}

#endif