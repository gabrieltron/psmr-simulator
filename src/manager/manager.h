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
    Manager(int n_variables);

    virtual ExecutionLog execute_requests() = 0;
    virtual void export_data(std::string output_path) = 0;

    void add_request(Request request);
    void set_requests(std::vector<Request> requests);
    void export_requests(std::ostream& output_stream);
    void import_requests(std::string input_path);

    int n_variables();

protected:
    int n_variables_{0};
    std::deque<Request> requests_;
};

}

#endif
