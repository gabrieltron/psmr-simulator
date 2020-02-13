#ifndef MODEL_GRAPH_H
#define MODEL_GRAPH_H

#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

// bad name, idk to what to change
namespace model {

typedef std::unordered_map<int, int> Vertex;
typedef std::unordered_map<int,std::unordered_map<int, int>> Edges;

class Graph {
public:
    Graph() = default;
    Graph(int n_vertex);

    void add_vertice(int id, int weight = 0);
    void remove_vertice(int vertice_id);
    void add_edge(int from, int to, int weight = 0);
    void remove_edge(int from, int to);
    bool are_connected(int vertice_a, int vertice_b);
    void increase_vertice_weight(int vertice, int value = 1);
    void set_vertice_weight(int vertice, int weight);
    void increase_edge_weight(int from, int to, int value = 1);

    std::size_t n_vertex();
    std::size_t n_edges();
    int total_vertex_weight();
    int total_edges_weight();
    int vertice_weight(int vertice);
    int edge_weight(int from, int to);
    std::unordered_map<int, int>& vertice_edges(int vertice);
    Vertex& vertex();
    int in_degree(int vertice);
    std::unordered_map<int, int> in_degrees() const;

protected:
    Vertex vertex_;
    Edges edges_;
    int n_edges_{0};
    int total_edges_weight_{0};
    int total_vertex_weight_{0};
    std::unordered_map<int, int> in_degree_;
};

}

#endif
