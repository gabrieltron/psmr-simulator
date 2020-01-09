#ifndef WORKLOAD_MANAGER_H
#define WORKLOAD_MANAGER_H

#include <deque>
#include <iostream>
#include <string>
#include <toml11/toml.hpp>
#include <unordered_map>
#include <unordered_set>

#include "execution_log.h"
#include "graph.h"
#include "graph_cut.h"
#include "partition_scheme.h"
#include "random.h"

namespace workload {

// A request is the set of the data involved in the request
// It creates limitations on import but causes random multi-data requests
// to be generated a bit faster.
typedef std::unordered_set<int> Request;

class Manager {
public:
    Manager(
        int n_variables,
        int n_partitions,
        std::vector<long int> data_partition
    );
    Manager(int n_variables, int n_partitions);

    void add_request(Request request);
    ExecutionLog execute_requests();
    void repartition_data(int n_partitions);
    void set_requests(std::vector<Request> requests);

    void export_requests(std::ostream& output_stream);
    void import_requests(std::string input_path);

    int n_variables();
    model::Graph access_graph();
    PartitionScheme partiton_scheme();

private:
    void update_access_graph(Request request);

    int n_variables_{0};
    std::deque<Request> requests_;
    PartitionScheme partition_scheme_;
    model::Graph access_graph_;
};

}

#endif
