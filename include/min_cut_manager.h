#ifndef WORKLOAD_MIN_CUT_MANAGER_H
#define WORKLOAD_MIN_CUT_MANAGER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "execution_log.h"
#include "graph.h"
#include "graph_cut.h"
#include "manager.h"
#include "partition_scheme.h"
#include "random.h"

namespace workload {

class MinCutManager : public Manager {
public:
    MinCutManager(
        int n_variables,
        int n_partitions,
        int repartition_interval,
        std::vector<long int> data_partition
    );
    MinCutManager(
        int n_variables,
        int n_partitions,
        int repartition_interval
    );

    ExecutionLog execute_requests();
    void repartition_data(int n_partitions);

    int n_variables();
    model::Graph access_graph();
    PartitionScheme partiton_scheme();

    void export_data(std::string output_path);

private:
    void update_access_graph(Request request);

    int repartition_interval_;
    PartitionScheme partition_scheme_;
    model::Graph access_graph_;
};

}

#endif
