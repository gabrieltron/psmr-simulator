#ifndef WORKLOAD_EARLY_MIN_CUT_MANAGER_H
#define WORKLOAD_EARLY_MIN_CUT_MANAGER_H

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

class EarlyMinCutManager : public Manager {
public:
    EarlyMinCutManager(
        int n_variables,
        int n_partitions,
        int repartition_window
    );

    ExecutionLog execute_requests();
    void update_graph(
        model::Graph& graph, const Request& request
    ) const;

    void export_data(std::string output_path);

protected:
    int repartition_window_;
    int n_partitions_;
};

}

#endif
