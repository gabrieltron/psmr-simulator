#include "manager.h"

namespace workload {

Manager::Manager(
    int n_variables,
    int n_partitions,
    std::vector<long int> data_partition)
    :   n_variables_{n_variables},
        access_graph_{model::Graph(n_variables)},
        partition_scheme_{PartitionScheme(n_partitions, data_partition)}
{}

// Distribute data in partitions with round-robin
Manager::Manager(int n_variables, int n_partitions)
    :   n_variables_{n_variables},
        access_graph_{model::Graph(n_variables)}
{
    auto data_partition = std::vector<long int>();
    auto current_partition = 0;
    for (auto i = 0; i < n_variables_; i++) {
        data_partition.push_back(current_partition);
        current_partition = (current_partition+1) % n_partitions;
    }
    partition_scheme_ = PartitionScheme(n_partitions, data_partition);
}

void Manager::add_request(Request request) {
    requests_.push_back(request);
}

ExecutionLog Manager::execute_requests(
    unsigned int repartition_interval /*= 0*/
) {
    auto log = ExecutionLog(partition_scheme_.n_partitions());

    while (!requests_.empty()) {
        auto request = requests_.front();
        requests_.pop_front();

        auto involved_partitions = std::unordered_set<int>();
        for (auto data : request) {
            involved_partitions.insert(
                partition_scheme_.data_partition(data)
            );
        }

        if (involved_partitions.size() == 1) {
            auto partition = *(involved_partitions.begin());
            log.increase_elapsed_time(partition);
        } else {
            log.sync_partitions(involved_partitions);
            for (auto partition : involved_partitions) {
                log.increase_elapsed_time(partition);
            }
        }

        update_access_graph(request);
        log.increase_processed_requests();
        if (repartition_interval != 0 and
            log.processed_requests() % repartition_interval == 0
        ) {
            repartition_data(partition_scheme_.n_partitions());
        }
    }

    return log;
}

void Manager::update_access_graph(Request request) {
    for (auto first_data : request) {
        access_graph_.increase_vertice_weight(first_data);
        for (auto second_data : request) {
            if (first_data == second_data) {
                continue;
            }

            if (!access_graph_.are_connected(first_data, second_data)) {
                access_graph_.add_edge(first_data, second_data);
                access_graph_.add_edge(second_data, first_data);
            }

            access_graph_.increase_edge_weight(first_data, second_data);
        }
    }
}

void Manager::repartition_data(int n_partitions) {
    auto data_partitions = model::cut_graph(access_graph_, n_partitions);
    partition_scheme_.update_partitions(n_partitions, data_partitions);
}

void Manager::export_requests(std::ostream& output_stream) {
    toml::array toml_array;
    for (auto request : requests_) {
        toml_array.push_back(request);
    }
    const toml::value data(toml_array);
    output_stream << "requests = ";
    output_stream << std::setw(80) << data << std::endl;
}

// Maybe import should be done by stream too,
// I'll do it with toml just for simplicity
// to reuse the parser
void Manager::import_requests(std::string input_path) {
    const auto requests_file = toml::parse(input_path);
    const auto requests_vector = toml::find<std::vector<std::vector<int>>>(requests_file, "requests");
    for (auto request_vector : requests_vector) {
        auto request = Request(request_vector.begin(), request_vector.end());
        requests_.push_back(request);
    }
}

void Manager::set_requests(std::vector<Request> requests) {
    for (auto request_vector : requests) {
        auto request = Request(request_vector.begin(), request_vector.end());
        requests_.push_back(request);
    }
}

model::Graph Manager::access_graph() {
    return access_graph_;
}

PartitionScheme Manager::partiton_scheme() {
    return partition_scheme_;
}

int Manager::n_variables() {
    return n_variables_;
}

}
