#ifndef WORKLOAD_REQUEST_GENERATOR_H
#define WORKLOAD_REQUEST_GENERATOR_H

#include <algorithm>
#include <fstream>
#include <functional>
#include <random>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "random.h"

namespace workload {

typedef std::unordered_set<int> Request;

std::vector<Request> import_requests(const std::string& input_path, int n_initial_keys);
std::vector<Request> generate_single_data_requests(
    int n_requests,
    rfunc::RandFunction& data_rand
);
std::vector<Request> generate_fixed_data_requests(
    int n_variables, int requests_per_variable
);
std::vector<Request> generate_multi_data_requests(
    int n_requests,
    int n_variables,
    rfunc::RandFunction& data_rand,
    rfunc::RandFunction& size_rand
);
void shuffle_requests(std::vector<Request>& requests);
std::vector<Request> merge_requests(
    std::vector<Request> single_data_requests,
    std::vector<Request> multi_data_requests,
    int single_data_pick_probability
);

}

#endif
