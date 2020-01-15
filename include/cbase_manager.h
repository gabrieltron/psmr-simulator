#ifndef WORKLOAD_CBASE_MANAGER_H
#define WORKLOAD_CBASE_MANAGER_H

#include <deque>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "execution_log.h"
#include "graph.h"
#include "manager.h"
#include "partition_scheme.h"
#include "random.h"

namespace workload {

typedef std::priority_queue<std::pair<int, int>> Heap;

class CBaseManager : Manager {
public:
    CBaseManager(int n_threads);

    ExecutionLog execute_requests();
    model::Graph generate_dependency_graph();

private:
    Heap initialize_threads_heap();
    Heap initialize_font_heap(const model::Graph& graph);

    int n_threads_;
    std::deque<Request> requests_;
    PartitionScheme partition_scheme_;
    model::Graph access_graph_;
};

}

#endif
