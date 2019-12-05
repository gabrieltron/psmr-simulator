#include "request_generator.h"

#include <iostream>

namespace workload {

RequestGenerator::RequestGenerator(int n_variables)
    : n_variables_{n_variables}
{
    single_data_rand_ = rfunc::get_random_function(rfunc::UNIFORM, n_variables-1);
    multi_data_rand_ = rfunc::get_random_function(rfunc::UNIFORM, n_variables-1);
}

void RequestGenerator::create_requests(Manager& manager) {
    auto created_single_data_requests = 0;
    auto created_multi_data_requests = 0;
    auto rand = rfunc::get_random_function(rfunc::UNIFORM, 100);
    auto available_variables = std::vector<int>(n_variables_, 0);
    update_n_single_data_requests();
    update_multi_data_rand_function();
    update_single_data_rand_function();

    while (created_single_data_requests < single_data_requests_ or
            created_multi_data_requests < multi_data_requests_
    ) {

        if (created_single_data_requests < single_data_requests_ and
            created_multi_data_requests < multi_data_requests_
        ) {

            auto n = rand();
            if (n <= single_data_spawn_chace_) {
                create_single_data_request(manager, available_variables);
                created_single_data_requests++;
            } else {
                create_multi_data_request(manager);
                created_multi_data_requests++;
            }

        } else if (created_single_data_requests < single_data_requests_) {
            create_single_data_request(manager, available_variables);
            created_single_data_requests++;
        } else {
            create_multi_data_request(manager);
            created_multi_data_requests++;
        }
    }

}

void RequestGenerator::create_single_data_request(
    Manager& manager, std::vector<int>& available_variables
) {
    if (single_data_request_distribution_ == rfunc::UNIFORM) {
        auto request = Request();
        auto data = single_data_rand_();
        request.insert(data);
        manager.add_request(request);
    } else {
        auto request = Request();
        auto data = single_data_rand_();
        while (available_variables[data] == single_requests_per_data_) {
            data = (data + 1) % n_variables_;
        }

        request.insert(data);
        manager.add_request(request);
        available_variables[data]++;
    }
}

void RequestGenerator::create_multi_data_request(Manager& manager) {
    if (multi_data_request_distribution_ == rfunc::UNIFORM) {
        auto n_involved_data = multi_data_rand_() + 2;  // + 2 is so it'll never be 1
        auto request = Request();
        for (auto i = 0; i < n_involved_data; i++) {
            auto data = single_data_rand_();
            while (request.find(data) != request.end()) {
                data = (data + 1) % n_variables_;
            }
            request.insert(data);
        }
        manager.add_request(request);
    } else {
        auto request = Request();
        for (auto i = 0; i < n_variables_; i++) {
            request.insert(i);
        }
        manager.add_request(request);
    }
}

void RequestGenerator::update_n_single_data_requests() {
    if (single_data_request_distribution_ == rfunc::FIXED) {
        single_data_requests_ = n_variables_ * single_requests_per_data_;
    }
}

void RequestGenerator::set_single_data_spawn_chance(int single_data_spawn_chance) {
    single_data_spawn_chace_ = single_data_spawn_chance;
}

void RequestGenerator::set_single_data_requests(int single_data_requests) {
    single_data_requests_ = single_data_requests;
}

void RequestGenerator::set_single_requests_per_data(int single_requests_per_data) {
    single_requests_per_data_ = single_requests_per_data;
}

void RequestGenerator::single_data_request_distribution(
    rfunc::Distribution single_data_request_distribution
) {
    single_data_request_distribution_ = single_data_request_distribution;
    update_single_data_rand_function();
}

void RequestGenerator::update_single_data_rand_function() {
    if (single_data_request_distribution_ != rfunc::FIXED) {
        single_data_rand_ = rfunc::get_random_function(
            single_data_request_distribution_, n_variables_-1
        );
    }
}

void RequestGenerator::set_multi_data_requests(int multi_data_requests) {
    multi_data_requests_ = multi_data_requests;
}

void RequestGenerator::set_max_involved_data(int max_involved_data) {
    max_involved_data_ = max_involved_data;
}

void RequestGenerator::multi_data_request_distribution(
    rfunc::Distribution multi_data_request_distribution
) {
    multi_data_request_distribution_ = multi_data_request_distribution;
}

void RequestGenerator::update_multi_data_rand_function() {
    if (multi_data_request_distribution_ != rfunc::FIXED) {
        multi_data_rand_ = rfunc::get_random_function(
            multi_data_request_distribution_, max_involved_data_ - 2
        );
    }
}

}