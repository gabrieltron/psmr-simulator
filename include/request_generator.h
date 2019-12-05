#ifndef WORKLOAD_REQUEST_GENERATOR_H
#define WORKLOAD_REQUEST_GENERATOR_H

#include <vector> 

#include "manager.h"
#include "random.h"

namespace workload {

class RequestGenerator {
public:
    RequestGenerator(int n_variables);

    void set_single_data_spawn_chance(int single_data_spawn_chance);
    void set_single_data_requests(int single_data_requests);
    void set_single_requests_per_data(int single_requests_per_data);
    void single_data_request_distribution(
        rfunc::Distribution single_data_request_distribution
    );

    void set_multi_data_requests(int multi_data_requests);
    void set_max_involved_data(int max_involved_data);
    void multi_data_request_distribution(
        rfunc::Distribution multi_data_request_distribution
    );

    void create_requests(Manager& manager);

private:
    void create_single_data_request(
        Manager& manager, std::vector<int>& available_variables
    );
    void create_multi_data_request(Manager& manager);
    void update_n_single_data_requests();
    void update_single_data_rand_function();
    void update_multi_data_rand_function();

    int n_variables_, single_data_spawn_chace_ = 100;
    int single_data_requests_ = 0, single_requests_per_data_ = 0;
    rfunc::Distribution single_data_request_distribution_ = rfunc::UNIFORM;
    rfunc::RandFunction single_data_rand_;

    int multi_data_requests_ = 0, max_involved_data_ = 0;
    rfunc::Distribution multi_data_request_distribution_ = rfunc::UNIFORM;
    rfunc::RandFunction multi_data_rand_;
};

}

#endif