#include "request_generation.h"

namespace workload {

std::vector<Request> generate_single_data_requests(
    int n_requests, rfunc::RandFunction& data_rand
) {
    auto requests = std::vector<Request>();
    for (auto i = 0; i < n_requests; i++) {
        auto request = Request();
        auto data = data_rand();
        request.insert(data);
        requests.push_back(request);
    }
    return requests;
}

std::vector<Request> generate_fixed_data_requests(
    int n_variables, int requests_per_variable
) {
    auto requests = std::vector<Request>();
    for (auto i = 0; i < n_variables; i++) {
        for (auto j = 0; j < requests_per_variable; j++) {
            auto request = Request();
            request.insert(i);
            requests.push_back(request);
        }
    }

    shuffle_requests(requests);
    return requests;
}

std::vector<Request> generate_multi_data_requests(
    int n_requests,
    int n_variables,
    rfunc::RandFunction& data_rand,
    rfunc::RandFunction& size_rand
) {
    auto requests = std::vector<Request>();

    for (auto i = 0; i < n_requests; i++) {
        auto request = Request();
        auto request_size = size_rand();

        for (auto j = 0; j < request_size; j++) {
            auto data = data_rand();
            while (request.find(data) != request.end()) {
                data = data_rand();
            }
            request.insert(data);
        }

        requests.push_back(request);
    }
    return requests;
}

void shuffle_requests(std::vector<Request>& requests) {
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(requests), std::end(requests), rng);
}

std::vector<Request> merge_requests(
    std::vector<Request> single_data_requests,
    std::vector<Request> multi_data_requests,
    int single_data_pick_probability
) {
    auto shuffled_requests = std::vector<Request>();
    auto rand = rfunc::uniform_distribution_rand(1, 100);
    std::reverse(single_data_requests.begin(), single_data_requests.end());
    std::reverse(multi_data_requests.begin(), multi_data_requests.end());

    while (!single_data_requests.empty() and !multi_data_requests.empty()) {
        auto random_value = rand();

        if (single_data_pick_probability > random_value) {
            shuffled_requests.push_back(single_data_requests.back());
            single_data_requests.pop_back();
        } else {
            shuffled_requests.push_back(multi_data_requests.back());
            multi_data_requests.pop_back();
        }

    }

    while (!single_data_requests.empty()) {
        shuffled_requests.push_back(single_data_requests.back());
        single_data_requests.pop_back();
    }
    while (!multi_data_requests.empty()) {
        shuffled_requests.push_back(multi_data_requests.back());
        multi_data_requests.pop_back();
    }

    return shuffled_requests;
}

}
