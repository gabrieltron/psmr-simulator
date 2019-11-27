#ifndef MODEL_GRAPH_H
#define MODEL_GRAPH_H

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

// bad name, idk to what to change
namespace model {

typedef std::unordered_map<int, int> Vertex;
typedef std::unordered_map<int,std::unordered_map<int, int>> Edges;

enum ExportFormat {METIS, DOT};
const std::unordered_map<std::string, ExportFormat> string_to_format({
    {"METIS", ExportFormat::METIS},
    {"DOT", ExportFormat::DOT}
});

class Graph {
public:
    Graph() = default;
    Graph(int n_vertex);

    void add_vertice(int id, int weight = 0);
    void add_edge(int from, int to, int weight = 0);
    bool are_connected(int vertice_a, int vertice_b);
    void increase_vertice_weight(int vertice);
    void increase_edge_weight(int from, int to);
    void export_graph(std::ostream& output_stream, ExportFormat format);
    void export_metis(std::ostream& output_stream);
    void export_dot(std::ostream& output_stream);

    std::size_t n_vertex();
    int vertice_weight(int vertice);
    std::unordered_map<int, int> vertice_edges(int vertice);
    Vertex vertex();

private:
    Vertex vertex_;
    Edges edges_;
    int n_edges_{0};
};

}

#endif
