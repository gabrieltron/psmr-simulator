#include <fstream>
#include <iostream>
#include <manager.h>
#include <string>
#include <toml11/toml.hpp>
#include <unordered_map>


typedef toml::basic_value<toml::discard_comments, std::unordered_map> toml_config;


void generate_random_requests(
    const toml_config& config, workload::Manager& manager
) {
    // Generate single data requisitions
    const auto single_data_distribution_ = toml::find<std::string>(
        config, "workload", "requests", "single_data", "distribution_pattern"
    );
    auto single_data_distribution = workload::string_to_distribution.at(
        single_data_distribution_
    );
    if (single_data_distribution != workload::Distribution::FIXED) {
        const auto n_requests = toml::find<int>(
            config, "workload", "requests", "single_data", "n_requests"
        );
        manager.create_single_data_random_requests(
            n_requests, single_data_distribution
        );
    }

    // Generate multiple data requisitions
    const auto multi_data_distribution_ = toml::find<std::string>(
        config, "workload", "requests", "multi_data", "distribution_pattern"
    );
    auto multi_data_distribution = workload::string_to_distribution.at(
        multi_data_distribution_
    );
    if (multi_data_distribution != workload::Distribution::FIXED) {
        const auto n_requests = toml::find<int>(
            config, "workload", "requests", "multi_data", "n_requests"
        );
        const auto max_involved_data = toml::find<int>(
            config, "workload", "requests", "multi_data",
            "max_involved_data"
        );
        manager.create_multi_data_random_requests(
            n_requests, multi_data_distribution, max_involved_data
        );
    }

}


void import_requests(const toml_config& config, workload::Manager& manager) {
    auto input_path = toml::find<std::string>(
        config, "workload", "requests", "input_path"
    );
    manager.import_requests(input_path);
}


int main(int argc, char* argv[]) {
    const auto config = toml::parse(argv[1]);

    // Creates the manager
    const auto n_variables = toml::find<int>(
        config, "workload", "n_variables"
    );
    auto manager = workload::Manager(n_variables);

    // Create/import requests
    const auto import_requests_ = toml::find<bool>(
        config, "workload", "requests", "import_requests"
    );
    if (import_requests_) {
        import_requests(config, manager);
    } else {
        generate_random_requests(config, manager);
    }

    // Export requests, if requested
    const auto export_requests = toml::find<bool>(
        config, "output", "requests", "export"
    );
    if (export_requests) {
        const auto output_path = toml::find<std::string>(
            config, "output", "requests", "output_path"
        );
        std::ofstream ofs(output_path, std::ofstream::out);
        manager.export_requests(ofs);
        ofs.close();
    }

    manager.execute_requests();

    // Export generated graph, if requested
    const auto export_graph = toml::find<bool>(
        config, "output", "graph", "export"
    );
    if (export_graph) {
        const auto output_path = toml::find<std::string>(
            config, "output", "graph", "output_path"
        );
        std::ofstream ofs(output_path, std::ofstream::out);
        manager.export_graph(ofs);
        ofs.close();
    }

    // cut graph
    const auto n_partitions = toml::find<int>(
        config, "graph", "n_partitions"
    );
    auto partition_scheme = manager.partition_graph(n_partitions);

    // export partitions
    const auto partitions_output_path = toml::find<std::string>(
        config, "output", "partitions_output"
    );
    std::ofstream ofs(partitions_output_path, std::ofstream::out);
    partition_scheme.export_scheme(ofs);
    ofs.close();

    return 0;
}
