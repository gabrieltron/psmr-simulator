#ifndef WORKLOAD_MIN_CUT_MANAGER_H
#define WORKLOAD_MIN_CUT_MANAGER_H

#include <iostream>
#include <metis.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "execution_log.h"
#include "graph.h"
#include "min_cut.h"
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
        std::vector<int> data_partition
    );
    MinCutManager(
        int n_variables,
        int n_partitions,
        int repartition_interval
    );

    ExecutionLog execute_requests();
    virtual void repartition_data(int n_partitions) = 0;

    PartitionScheme partiton_scheme();
    void export_data(std::string output_path);

protected:
    virtual void update_access_structure(Request request) = 0;

    int repartition_interval_;
    PartitionScheme partition_scheme_;
};

}

#endif
