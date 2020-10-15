#ifndef WORKLOAD_GRAPH_CUT_MANAGER_H
#define WORKLOAD_GRAPH_CUT_MANAGER_H

#include <vector>

#include "graph/graph.h"
#include "partition/min_cut.h"
#include "min_cut_manager.hpp"

namespace workload {

class GraphCutManager : public MinCutManager {
public:
    GraphCutManager() = default;
    GraphCutManager(
        int n_variables,
        int n_partitions,
        int repartition_interval,
        std::vector<int> data_partition,
        model::CutMethod cut_method
    );
    GraphCutManager(
        int n_variables,
        int n_partitions,
        int repartition_interval,
        model::CutMethod cut_method
    );

    void set_cut_method(model::CutMethod cut_method);
    void repartition_data(int n_partitions);
    void export_data(std::string output_path);

private:
    model::CutMethod cut_method_;

};

}

#endif
