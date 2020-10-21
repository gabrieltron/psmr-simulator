#ifndef OUTPUT_WRITE_TO_FILE_H
#define OUTPUT_WRITE_TO_FILE_H

#include <iostream>
#include <metis.h>
#include <string>
#include <unordered_map>

#include "log/execution_log.h"
#include "graph/graph.h"
#include "partition/min_cut.h"
#include "partition/partition_manager.h"
#include "graph/spanning_tree.h"

namespace output {

enum GraphFormats {METIS, DOT};
const std::unordered_map<std::string, GraphFormats> string_to_format({
    {"METIS", GraphFormats::METIS},
    {"DOT", GraphFormats::DOT}
});

void write_graph(
    model::Graph& graph, GraphFormats format, std::ostream& output_stream
);
void write_metis_format(model::Graph& graph, std::ostream& output_stream);
void write_dot_format(model::Graph& graph, std::ostream& output_stream);
void write_cut_info(
    model::Graph& graph,
    workload::PartitionManager& partition_scheme,
    std::ostream& output_stream
);
void write_log_info(
    const workload::ExecutionLog& execution_log,
    std::ostream& output_stream
);
void write_data_partitions(
    std::unordered_map<int, int>,
    std::ostream& output_stream
);
void write_spanning_tree(
    const model::SpanningTree& tree,
    std::ostream& output_stream
);

void write_makespan(
    const workload::ExecutionLog& execution_log,
    std::ostream& output_stream
);

void write_requests_executed_per_partition(
    const workload::ExecutionLog& execution_log,
    std::ostream& output_stream
);

void write_execution_and_idle_time_per_thread(
    const workload::ExecutionLog& execution_log,
    std::ostream& output_stream
);

void write_idle_time(
    const workload::ExecutionLog& execution_log,
    std::ostream& output_stream
);

void write_syncronization_info(
    const workload::ExecutionLog& execution_log,
    std::ostream& output_stream
);

}

#endif
