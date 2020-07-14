#ifndef WORKLOAD_CBASE_MANAGER_H
#define WORKLOAD_CBASE_MANAGER_H

#include <deque>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "log/execution_log.h"
#include "graph/graph.h"
#include "manager.h"
#include "partition/partition_scheme.h"
#include "request/random.h"


namespace workload {

typedef std::priority_queue<
    std::pair<int, int>,
    std::vector<std::pair<int, int>>,
    std::greater<std::pair<int, int>>
> Heap;

class CBaseManager : public Manager {
public:
    CBaseManager(int n_variables, int n_threads);

    ExecutionLog execute_requests();
    model::Graph generate_dependency_graph();
    void export_data(std::string output_path);

private:
    Heap initialize_threads_heap();
    Heap initialize_font_heap(const model::Graph& graph);

    int n_threads_;
};

}

#endif
