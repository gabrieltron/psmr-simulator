#ifndef WORKLOAD_GRAPH_CUT_MANAGER_H
#define WORKLOAD_GRAPH_CUT_MANAGER_H

#include <vector>

#include "graph.h"
#include "min_cut.h"
#include "min_cut_manager.h"

namespace workload {

class GraphCutManager : public MinCutManager {
public:
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

    model::Graph access_graph();
    void repartition_data(int n_partitions);
    void export_data(std::string output_path);

private:
    void update_access_structure(Request request);

    model::Graph access_graph_;
    model::CutMethod cut_method_;

};

}

#endif
