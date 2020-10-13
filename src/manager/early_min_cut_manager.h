#ifndef WORKLOAD_EARLY_MIN_CUT_MANAGER_H
#define WORKLOAD_EARLY_MIN_CUT_MANAGER_H

#include <metis.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "log/execution_log.h"
#include "graph/graph.h"
#include "partition/min_cut.h"
#include "manager.h"
#include "request/random.h"

namespace workload {

class EarlyMinCutManager : public Manager {
public:
    EarlyMinCutManager() = default;
    EarlyMinCutManager(
        int n_variables,
        int n_partitions,
        int repartition_window
    );

    ExecutionLog execute_requests();
    void update_graph(
        model::Graph& graph, const Request& request
    ) const;

    void set_repartition_window(int repartition_window);
    void set_n_partitions(int n_partitions);

    void export_data(std::string output_path);

protected:
    int repartition_window_;
    int n_partitions_;
};

}

#endif
