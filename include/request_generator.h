#ifndef MODEL_REQUEST_GENERATOR_H
#define MODEL_REQUEST_GENERATOR_H

#include "manager.h"
#include "random.h"

namespace model {

void add_requests(
    workload::Manager& manager, int single_data_spawn_chance,
    rfunc::Distribution single_access_distribution, int n_single_requests,
    rfunc::Distribution multi_access_distribution, int n_multi_requests,
    int max_involved_data, int requests_per_data=0, int n_all_data_requests=0
);

}

#endif