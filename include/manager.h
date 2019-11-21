#ifndef WORKLOAD_MANAGER_H
#define WORKLOAD_MANAGER_H

#include <deque>
#include <functional>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <toml11/toml.hpp>
#include <unordered_map>
#include <unordered_set>

#include "graph.h"
#include "graph_cut.h"
#include "partition_scheme.h"

namespace workload {

// A request is the set of the data involved in the request
// It creates limitations on import but causes random multi-data requests
// to be generated a bit faster.
typedef std::unordered_set<int> Request;

enum Distribution {FIXED, UNIFORM};
const std::unordered_map<std::string, Distribution> string_to_distribution({
    {"FIXED", Distribution::FIXED},
    {"UNIFORM", Distribution::UNIFORM}
});

std::function<int()> get_random_function(
    Distribution distribution, int max_value
);

class Manager {
public:
    Manager() = default;
    Manager(int n_variables);

    void create_single_data_random_requests(
        int n_requests,
        Distribution distribution_pattern
    );
    void create_multi_data_random_requests(
        int n_requests,
        Distribution distribution_pattern,
        int max_involved_vertex
    );
    void execute_requests();
    model::PartitionScheme partition_graph(int n_partitions);
    void export_requests(std::ostream& output_stream);
    void import_requests(std::string input_path);
    void export_graph(std::ostream& output_stream);

private:
    int n_variables_{0};
    std::deque<Request> requests_;
    model::Graph access_graph_;
};

}

#endif
