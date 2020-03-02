#ifndef OUTPUT_WRITE_TO_FILE_H
#define OUTPUT_WRITE_TO_FILE_H

#include <iostream>
#include <metis.h>
#include <string>
#include <unordered_map>

#include "execution_log.h"
#include "graph.h"
#include "min_cut.h"
#include "partition_scheme.h"
#include "spanning_tree.h"

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
    workload::PartitionScheme& partition_scheme,
    std::ostream& output_stream
);
void write_log_info(
    workload::ExecutionLog& execution_log,
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

}

#endif