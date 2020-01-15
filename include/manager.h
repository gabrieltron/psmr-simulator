#ifndef WORKLOAD_MANAGER_H
#define WORKLOAD_MANAGER_H

#include <toml11/toml.hpp>

#include "execution_log.h"

namespace workload {

typedef std::unordered_set<int> Request;

class Manager {
public:
    virtual ExecutionLog execute_requests() = 0;

    void add_request(Request request);
    void set_requests(std::vector<Request> requests);
    void export_requests(std::ostream& output_stream);
    void import_requests(std::string input_path);
    
    int n_variables();

private:
    int n_variables_{0};
};

}

#endif
