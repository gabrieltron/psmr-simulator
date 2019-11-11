#include <cstddef>
#include <iostream>
#include <metis.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>


typedef std::unordered_map<int,std::unordered_map<int, int>> Edges;
typedef std::unordered_map<int, int> Vertex;
typedef std::unordered_map<int, int> PartitionSchemeInfo;


struct Graph {
    Graph() = default;

    Graph(int n_vertex) {
        for (auto i = 0; i < n_vertex; i++) {
            vertex[i] = 0;
            edges[i] = std::unordered_map<int, int>();
        }
    }

    std::size_t n_vertex() {
        return vertex.size();
    }

    void add_vertice(int id, int weight) {
        vertex[id] = weight;
    }

    void add_edge(int from, int to, int weight) {
        if (edges[from].find(to) == edges[from].end()) {
            edges[from][to] = 0;
        }
        if (edges[to].find(from) == edges[to].end()) {
            edges[to][from] = 0;
        }

        edges[from][to] = weight;
        edges[to][from] = weight;
    }

    bool are_connected(int vertice_a, int vertice_b) {
        return edges[vertice_a].find(vertice_b) != edges[vertice_a].end();
    }

    Vertex vertex = Vertex();
    Edges edges = Edges();
};


struct PartitionScheme {
    PartitionScheme(Graph& og_graph, std::vector<idx_t>& vertex_partitions) {
        for(auto vertice = 0; vertice < vertex_partitions.size(); vertice++) {
            auto partition = vertex_partitions[vertice];

            if (partitions.find(partition) == partitions.end()) {
                partitions[partition] = Graph();
            }

            auto weight = og_graph.vertex[vertice];
            partitions[partition].add_vertice(vertice, weight);

            for (auto tuple: og_graph.edges[vertice]) {
                auto neighbour = tuple.first;
                auto edge_weight = tuple.second;

                if (vertex_partitions[neighbour] != partition) {
                    continue;
                }
                
                partitions[partition].add_edge(
                    vertice, neighbour, edge_weight
                );
            }
        }
    }

    void info(PartitionSchemeInfo& vertex_weight, PartitionSchemeInfo& edges_weight) {
        for (auto tuple: partitions) {
            auto partition_id = tuple.first;
            auto partition_graph = tuple.second;

            vertex_weight[partition_id] = 0;
            edges_weight[partition_id] = 0;

            for (auto v_tuple: partition_graph.vertex) {
                auto vertice = v_tuple.first;
                auto weight = v_tuple.second;

                vertex_weight[partition_id] += weight;

                for (auto e_tuple: partition_graph.edges[vertice]) {
                    auto neighbour = e_tuple.first;
                    auto edge_weight = e_tuple.second;

                    if (vertice < neighbour) {
                        edges_weight[partition_id] +=edge_weight;
                    }
                }
            } 
        }
    }

    std::unordered_map<int, Graph> partitions;
};


void simulate_single_data_access(Graph& graph, int n_accesses) {
    srand(time(NULL));
    for (auto i = 0; i < n_accesses; i++) {
        auto vertice = rand() % graph.n_vertex();
        graph.vertex[vertice]++;
    }
}


void simulate_k_data_access(Graph& graph, int k, int n_accesses, int access_weight) {
    srand(time(NULL));
    for (auto i = 0; i < n_accesses; i++) {
        auto vertex = std::vector<int>();
        for (auto j = 0; j < k; j++) {
            auto vertice = rand() % graph.n_vertex();
            vertex.push_back(vertice);
        }

        for (auto from_vertice: vertex) {
            graph.vertex[from_vertice] += access_weight;

            for (auto to_vertice: vertex) {
                if (from_vertice == to_vertice) {
                    continue;
                }

                if (!graph.are_connected(from_vertice, to_vertice)) {
                    graph.add_edge(from_vertice, to_vertice, 0);
                }
                graph.edges[from_vertice][to_vertice] += access_weight;
                graph.edges[to_vertice][from_vertice] += access_weight;
            }
        }
    }
}


PartitionScheme cut_graph(Graph& graph, idx_t n_partitions) {
    auto vertex = graph.vertex;
    idx_t n_vertice = vertex.size();
    idx_t n_edges = n_vertice * (n_vertice - 1);
    idx_t n_constrains = 1;

    auto vertice_weight = std::vector<idx_t>();
    for (auto i = 0; i < vertex.size(); i++) {
        vertice_weight.push_back(vertex[i]);
    }

    auto x_edges = std::vector<idx_t>();
    for (auto i = 0; i <= n_edges; i += n_vertice - 1) {
        x_edges.push_back(i);
    }

    auto edges = std::vector<idx_t>();
    auto edges_weight = std::vector<idx_t>();
    for (auto i = 0; i < n_vertice; i++) {
        for (auto j = 0; j < n_vertice; j++) {
            if (i == j) {
                continue;
            }

            edges.push_back(j);
            if (graph.edges[i].find(j) != graph.edges[i].end()) {
                auto edge_weight = graph.edges[i][j] + 1;
                edges_weight.push_back(edge_weight);
            } else {
                edges_weight.push_back(1);
            }
        }
    }

    idx_t options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);
    options[METIS_OPTION_OBJTYPE] = METIS_OBJTYPE_CUT;
    options[METIS_OPTION_NUMBERING] = 0;
    //options[METIS_OPTION_DBGLVL] = 511;

    idx_t objval;
    auto vertex_partitions = std::vector<idx_t>(n_vertice, 0);
    METIS_PartGraphKway(
        &n_vertice, &n_constrains, x_edges.data(), edges.data(),
        vertice_weight.data(), NULL, edges_weight.data(), &n_partitions, NULL,
        NULL, options, &objval, vertex_partitions.data()
    );
    
    auto partition_scheme = PartitionScheme(graph, vertex_partitions);
    return partition_scheme;
}


int main(int argc, char *argv[]) {

    if (argc != 7) {
        std::cout << "Wrong number of args\n";
        return 0;
    }

    auto n_vertex = atoi(argv[1]);
    auto n_accesses = atoi(argv[2]);
    auto k = atoi(argv[3]);
    auto n_k_accesses = atoi(argv[4]);
    auto access_weight = atoi(argv[5]);
    auto n_partitions = atoi(argv[6]);

    auto graph = Graph(n_vertex);
    simulate_single_data_access(graph, n_accesses);
    simulate_k_data_access(graph, k, n_k_accesses, access_weight);

    auto partition_scheme = cut_graph(graph, n_partitions);

    auto partitions_weight = PartitionSchemeInfo();
    auto partitions_edges_weight = PartitionSchemeInfo();
    partition_scheme.info(
        partitions_weight, partitions_edges_weight
    );

    std::cout << "Partição | Peso Vértices | Peso Arestas | Vértices\n"; 
    for (auto i = 0; i < n_partitions; i++) {
        if (partitions_weight.find(i) == partitions_weight.end()) {
            std::cout << i << " N.D. N.D. N.D\n";
            continue;
        }

        std::cout << i << " | " << partitions_weight[i]; 
        std::cout << " | " << partitions_edges_weight[i] << " |";
        for (auto tuple: partition_scheme.partitions[i].vertex) {
            auto vertice = tuple.first;
            std::cout << " " << vertice; 
        }
        std::cout << "\n";
    }
}