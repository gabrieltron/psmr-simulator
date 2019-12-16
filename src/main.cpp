#include <fstream>
#include <iostream>
#include <manager.h>
#include <string>
#include <toml11/toml.hpp>
#include <unordered_map>

#include "execution_log.h"
#include "graph_cut.h"
#include "request_generator.h"
#include "write.h"

typedef toml::basic_value<toml::discard_comments, std::unordered_map> toml_config;


void generate_random_requests(
    const toml_config& config, workload::Manager& manager
) {
    auto request_generator = workload::RequestGenerator(manager.n_variables());

    const auto single_data_spawn_chance = toml::find<int>(
        config, "workload", "requests", "single_data_spawn_chance"
    );
    request_generator.set_single_data_spawn_chance(single_data_spawn_chance);

    // Single data config
    const auto single_data_distribution_ = toml::find<std::string>(
        config, "workload", "requests", "single_data", "distribution_pattern"
    );
    auto single_data_distribution = rfunc::string_to_distribution.at(
        single_data_distribution_
    );
    request_generator.single_data_request_distribution(
        single_data_distribution
    );

    if (single_data_distribution != rfunc::FIXED) {
        const auto n_requests = toml::find<int>(
            config, "workload", "requests", "single_data", "n_requests"
        );
        request_generator.set_single_data_requests(n_requests);
    } else {
        const auto requests_per_data = toml::find<int>(
            config, "workload", "requests", "single_data", "requests_per_data"
        );
        request_generator.set_single_requests_per_data(requests_per_data);
    }

    // Multi data config
    const auto multi_data_distribution_ = toml::find<std::string>(
        config, "workload", "requests", "multi_data", "distribution_pattern"
    );
    auto multi_data_distribution = rfunc::string_to_distribution.at(
        multi_data_distribution_
    );
    request_generator.multi_data_request_distribution(multi_data_distribution);

    const auto n_requests = toml::find<int>(
        config, "workload", "requests", "multi_data", "n_requests"
    );
    request_generator.set_multi_data_requests(n_requests);

    if (multi_data_distribution != rfunc::FIXED) {
        const auto max_involved_data = toml::find<int>(
            config, "workload", "requests", "multi_data",
            "max_involved_data"
        );
        request_generator.set_max_involved_data(max_involved_data);
    }

    request_generator.create_requests(manager);
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
        config, "workload", "initial_partitions", "n_partitions"
    );
    const auto should_import_partitions = toml::find<bool>(
        config, "workload", "initial_partitions", "import"
    );

    if (should_import_partitions) {
        const auto path = toml::find<std::string>(
            config, "workload", "initial_partitions", "path"
        );
        const auto initial_partitions_file = toml::parse(path);
        auto data_partitions = toml::find<std::vector<long int>>(
            initial_partitions_file, "data_partitions"
        );
        return workload::Manager(n_variables, n_partitions, data_partitions);
    }

    return workload::Manager(n_variables, n_partitions);
}

void export_requests(const toml_config& config, workload::Manager& manager) {
    const auto output_path = toml::find<std::string>(
        config, "output", "requests", "path"
    );
    std::ofstream ofs(output_path, std::ofstream::out);
    manager.export_requests(ofs);
    ofs.close();
}

void export_workload_graph(const toml_config& config, workload::Manager& manager) {
    const auto str_format = toml::find<std::string>(
        config, "output", "workload_graph", "format"
    );
    auto format = output::string_to_format.at(str_format);
    const auto path = toml::find<std::string>(
        config, "output", "workload_graph", "path"
    );
    std::ofstream output_stream(path, std::ofstream::out);
    auto graph = manager.access_graph();
    output::write_graph(graph, format, output_stream);
    output_stream.close();
}

void export_cut_info(workload::Manager& manager, std::ostream& output_stream) {
    auto og_graph = manager.access_graph();
    auto partition_scheme = manager.partiton_scheme();
    output::write_cut_info(og_graph, partition_scheme, output_stream);
}

void export_execution_info(
    workload::ExecutionLog& execution_log,
    std::ostream& output_stream
) {
    output::write_log_info(execution_log, output_stream);
}

void export_execution_and_cut_info(
    const toml_config& config,
    workload::Manager& manager,
    workload::ExecutionLog& execution_log
) {
    const auto output_path = toml::find<std::string>(
        config, "output", "info_path"
    );
    std::ofstream output_stream(output_path, std::ofstream::out);
    export_execution_info(execution_log, output_stream);
    export_cut_info(manager, output_stream);
    output_stream.close();
}

void export_resulting_partition_graph(
    const toml_config& config, workload::Manager& manager
) {
    const auto graph_output_path = toml::find<std::string>(
        config, "output", "partition", "graph", "path"
    );
    auto graph = manager.partiton_scheme().graph_representation();
    std::ofstream parition_graph_stream(graph_output_path, std::ofstream::out);
    output::write_dot_format(graph, parition_graph_stream);
    parition_graph_stream.close();
}

void export_data_partitions(
    const toml_config& config, workload::PartitionScheme& partition_scheme
) {
    const auto output_path = toml::find<std::string>(
        config, "output", "partition", "data_partitions", "path"
    );
    auto data_partitions = partition_scheme.data_partition_vector();
    std::ofstream output_stream(output_path, std::ofstream::out);
    output::write_data_partitions(data_partitions, output_stream);
    output_stream.close();
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

    auto execution_log = manager.execute_requests();

    const auto should_export_workload_graph = toml::find<bool>(
        config, "output", "workload_graph", "export"
    );
    if (should_export_workload_graph) {
        export_workload_graph(config, manager);
    }
    const auto n_partitions = toml::find<int>(
        config, "cut", "n_partitions"
    );
    manager.repartition_data(n_partitions);
    export_execution_and_cut_info(config, manager, execution_log);

    const auto should_export_partition_graph = toml::find<bool>(
        config, "output", "partition", "graph", "export"
    );
    if (should_export_partition_graph) {
        export_resulting_partition_graph(config, manager);
    }

    const auto should_export_data_partitions = toml::find<bool>(
        config, "output", "partition", "data_partitions", "export"
    );
    if (should_export_data_partitions) {
        auto partition_scheme = manager.partiton_scheme();
        export_data_partitions(config, partition_scheme);
    }

    return 0;
}
