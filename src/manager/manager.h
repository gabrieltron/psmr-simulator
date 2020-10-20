#ifndef WORKLOAD_MANAGER_H
#define WORKLOAD_MANAGER_H

#include <deque>
#include <string>
#include <toml11/toml.hpp>

#include "request/request_generation.h"
#include "log/execution_log.h"
#include "write/write.h"

namespace workload {

class Manager {
public:
    Manager() = default;
    Manager(int n_variables);

    virtual ExecutionLog execute_requests() = 0;
    virtual void export_data(std::string output_path) = 0;

    void add_request(Request request);
    void set_requests(const std::vector<Request>& requests);
    void set_requests_execution_time(int requests_execution_time);
    void export_requests(std::ostream& output_stream);
    void import_requests(std::string input_path);

    int n_variables();
    void set_n_variables(int n_variables);

protected:
    int n_variables_{0};
    int requests_execution_time_{1};
    std::deque<Request> requests_;
};

}

#endif
