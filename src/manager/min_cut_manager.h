#ifndef WORKLOAD_MIN_CUT_MANAGER_H
#define WORKLOAD_MIN_CUT_MANAGER_H

#include <metis.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "log/execution_log.h"
#include "graph/graph.h"
#include "partition/min_cut.h"
#include "manager.h"
#include "partition/partition_manager.h"
#include "request/random.h"

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

    PartitionManager partition_manager();
    void export_data(std::string output_path);

protected:
    virtual void update_access_structure(const Request& request) = 0;

    int round_robin_counter_ = 0;
    int repartition_interval_;
    PartitionManager partition_manager_;
};

}

#endif
