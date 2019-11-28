#ifndef OUTPUT_WRITE_TO_FILE_H
#define OUTPUT_WRITE_TO_FILE_H

#include <fstream>
#include <string>
#include <unordered_map>

#include "graph.h"

namespace output {

enum GraphFormats {METIS, DOT};
const std::unordered_map<std::string, GraphFormats> string_to_format({
    {"METIS", GraphFormats::METIS},
    {"DOT", GraphFormats::DOT}
});

void write_graph(model::Graph& graph, GraphFormats format, std::string path);
void write_metis_format(model::Graph& graph, std::string path);
void write_dot_format(model::Graph& graph, std::string path);

}

#endif