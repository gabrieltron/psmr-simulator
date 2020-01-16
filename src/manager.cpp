#include "manager.h"

namespace workload {

Manager::Manager(int n_variables):
    n_variables_{n_variables}
{}

void Manager::add_request(Request request) {
    requests_.push_back(request);
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

int Manager::n_variables() {
    return n_variables_;
}

}
