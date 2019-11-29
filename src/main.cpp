#include <fstream>
#include <iostream>
#include <manager.h>
#include <string>
#include <toml11/toml.hpp>
#include <unordered_map>

#include "graph_cut.h"
#include "write.h"

typedef toml::basic_value<toml::discard_comments, std::unordered_map> toml_config;


void generate_random_requests(
    const toml_config& config, workload::Manager& manager
) {
    // Generate single data requisitions
    const auto single_data_distribution_ = toml::find<std::string>(
        config, "workload", "requests", "single_data", "distribution_pattern"
    );
    auto single_data_distribution = rfunc::string_to_distribution.at(
        single_data_distribution_
    );
    if (single_data_distribution != rfunc::Distribution::FIXED) {
        const auto n_requests = toml::find<int>(
            config, "workload", "requests", "single_data", "n_requests"
        );
        manager.create_single_data_random_requests(
            n_requests, single_data_distribution
        );
    } else {
        const auto requests_per_data = toml::find<int>(
            config, "workload", "requests", "single_data", "requests_per_data"
        );
        manager.create_fixed_quantity_requests(requests_per_data);
    }

    // Generate multiple data requisitions
    const auto multi_data_distribution_ = toml::find<std::string>(
        config, "workload", "requests", "multi_data", "distribution_pattern"
    );
    auto multi_data_distribution = rfunc::string_to_distribution.at(
        multi_data_distribution_
    );
    if (multi_data_distribution != rfunc::Distribution::FIXED) {
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
    } else {
        const auto n_all_data_requests = toml::find<int>(
            config, "workload", "requests", "multi_data", "n_all_data_requests"
        );
        manager.create_multi_all_data_requests(n_all_data_requests);
    }

}

void import_requests(const toml_config& config, workload::Manager& manager) {
    auto input_path = toml::find<std::string>(
        config, "workload", "requests", "input_path"
    );
    manager.import_requests(input_path);
}

workload::Manager create_manager(const toml_config& config) {
    const auto n_variables = toml::find<int>(
        config, "workload", "n_variables"
    );
    const auto n_partitions = toml::find<int>(
        config, "workload", "n_partitions"
    );
    const auto str_partitions_distribution = toml::find<std::string>(
        config, "workload", "partitions_distribution"
    );
    auto partitions_distribution = rfunc::string_to_distribution.at(
        str_partitions_distribution
    );
    if (partitions_distribution == rfunc::Distribution::FIXED) {
        const auto initial_partition = toml::find<std::vector<long int>>(
            config, "workload", "initial_partition"
        );
        return workload::Manager(n_variables, n_partitions, initial_partition);
    }
}

void export_requests(const toml_config& config, workload::Manager& manager) {
    const auto output_path = toml::find<std::string>(
        config, "output", "requests", "output_path"
    );
    std::ofstream ofs(output_path, std::ofstream::out);
    manager.export_requests(ofs);
    ofs.close();
}

void export_workload_graph(const toml_config& config, workload::Manager& manager) {
    const auto str_format = toml::find<std::string>(
        config, "output", "graph", "format"
    );
    auto format = output::string_to_format.at(str_format);
    const auto path = toml::find<std::string>(
        config, "output", "graph", "output_path"
    );
    std::ofstream output_stream(path, std::ofstream::out);
    auto graph = manager.access_graph();
    output::write_graph(graph, format, output_stream);
    output_stream.close();
}

void export_cut_info(const toml_config& config, workload::Manager& manager) {
    const auto info_output_path = toml::find<std::string>(
        config, "output", "partitions", "info_output_path"
    );
    auto og_graph = manager.access_graph();
    auto partition_scheme = manager.partiton_scheme();
    std::ofstream cut_info_stream(info_output_path, std::ofstream::out);
    output::write_cut_info(og_graph, partition_scheme, cut_info_stream);
    cut_info_stream.close();
}

void export_resulting_partition_graph(
    const toml_config& config, workload::Manager& manager
) {
    const auto graph_output_path = toml::find<std::string>(
        config, "output", "partitions", "graph_output_path"
    );
    auto graph = manager.partiton_scheme().graph_representation();
    std::ofstream parition_graph_stream(graph_output_path, std::ofstream::out);
    output::write_dot_format(graph, parition_graph_stream);
}

int main(int argc, char* argv[]) {
    const auto config = toml::parse(argv[1]);

    auto manager = create_manager(config);
    const auto should_import_requests = toml::find<bool>(
        config, "workload", "requests", "import_requests"
    );
    if (should_import_requests) {
        import_requests(config, manager);
    } else {
        generate_random_requests(config, manager);
    }

    const auto should_export_requests = toml::find<bool>(
        config, "output", "requests", "export"
    );
    if (should_export_requests) {
        export_requests(config, manager);
    }

    manager.execute_requests();

    const auto should_export_workload_graph = toml::find<bool>(
        config, "output", "graph", "export"
    );
    if (should_export_workload_graph) {
        export_workload_graph(config, manager);
    }

    const auto n_partitions = toml::find<int>(
        config, "graph", "n_partitions"
    );
    manager.repartition_data(n_partitions);
    export_cut_info(config, manager);
    export_resulting_partition_graph(config, manager);

    return 0;
}
