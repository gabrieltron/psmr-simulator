#include "min_cut.h"

namespace model {

workload::PartitionScheme cut_graph(
    CutMethod cut_method, Graph& graph, idx_t n_partitions
) {
    if (cut_method == METIS) {
        return metis_cut(graph, n_partitions);
    } else {
        return fennel_cut(graph, n_partitions);
    }
}

workload::PartitionScheme metis_cut(Graph& graph, idx_t n_partitions) {
    auto vertex = graph.vertex();
    idx_t n_vertice = vertex.size();
    idx_t n_edges = n_vertice * (n_vertice - 1);
    idx_t n_constrains = 1;

    auto vertice_weight = std::vector<idx_t>();
    for (auto i = 0; i < vertex.size(); i++) {
        vertice_weight.push_back(vertex[i]);
    }

    auto x_edges = std::vector<idx_t>();
    auto edges = std::vector<idx_t>();
    auto edges_weight = std::vector<idx_t>();

    x_edges.push_back(0);
    for (auto vertice = 0; vertice < vertex.size(); vertice++) {
        auto last_edge_index = x_edges.back();
        auto n_neighbours = graph.vertice_edges(vertice).size();
        x_edges.push_back(last_edge_index + n_neighbours);

        for (auto vk: graph.vertice_edges(vertice)) {
            auto neighbour = vk.first;
            auto weight = vk.second;
            edges.push_back(neighbour);
            edges_weight.push_back(weight);
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
    
    auto partitions = std::vector<workload::Partition>();
    for (auto i = 0; i < n_partitions; i++) {
        auto partition = workload::Partition();
        partitions.push_back(partition);
    }
    for (auto i = 0; i < vertex_partitions.size(); i++) {
        auto vertice = i;
        auto vertice_partition = vertex_partitions[vertice];

        auto vertice_weight = graph.vertice_weight(vertice);
        partitions[vertice_partition].insert(vertice, vertice_weight);
    }

    return workload::PartitionScheme(partitions);
;
}

workload::PartitionScheme fennel_cut(Graph& graph, int n_partitions) {
    auto partitions = std::vector<workload::Partition>();
    for (auto i = 0; i < n_partitions; i++) {
        auto partition = workload::Partition();
        partitions.push_back(partition);
    }

    const auto edges_weight = graph.total_edges_weight();
    const auto vertex_weight = graph.total_vertex_weight();
    const auto gamma = 3 / 2.0;
    const auto alpha =
        edges_weight * std::pow(partitions.size(), (gamma - 1)) / std::pow(graph.total_vertex_weight(), gamma);

    for (auto kv: graph.vertex()) {
        auto vertice = kv.first;
        //std::cout << "Analizing vertice " << vertice << "\n";
        auto partition = fennel_vertice_partition(graph, vertice, partitions, gamma);
        partitions[partition].insert(vertice, graph.vertice_weight(vertice));
    }

    return workload::PartitionScheme(partitions);
}

int fennel_inter_cost(
    const std::unordered_map<int, int>& edges, workload::Partition& partition
) {
    auto& vertex = partition.vertex();
    auto cost = 0;
    for (auto kv : edges) {
        auto vertice = kv.first;
        auto weight = kv.second;
        if (vertex.find(vertice) != vertex.end()) {
            cost += weight;
        }
    }
    return cost;
}

int biggest_value_index(std::vector<double>& partitions_score) {
    auto index = 0;
    auto biggest = partitions_score[0];
    for (auto i = 1; i < partitions_score.size(); i++) {
        if (partitions_score[i] > biggest) {
            biggest = partitions_score[i];
            index = i;
        }
    }
    return index;
}

int fennel_vertice_partition(
    Graph& graph, int vertice, std::vector<workload::Partition>& partitions,
    double gamma
) {
    double biggest_score = -DBL_MAX;
    auto id = 0;
    auto designated_partition = 0;
    for (auto partition : partitions) {
        auto partition_weight = partition.weight();
        auto& edges = graph.vertice_edges(vertice);

        auto inter_cost = fennel_inter_cost(edges, partition);
        auto intra_cost = 
            (std::pow(partition.weight() + graph.vertice_weight(vertice), gamma));
        intra_cost -= std::pow(partition.weight(), gamma);
        intra_cost *= gamma;
        auto score = inter_cost - intra_cost;

        if (score > biggest_score) {
            biggest_score = score;
            designated_partition = id;
        }
        id++;
    }

    return designated_partition;
}

workload::PartitionScheme spanning_tree_cut(SpanningTree& tree, int n_partitions) {
    auto partitions = std::vector<workload::Partition>();
    for (auto i = 0; i < n_partitions; i++) {
        auto partition = workload::Partition();
        partitions.push_back(partition);
    }

    auto detatched_ids = std::vector<std::pair<double, int>>();
    auto nodes = std::vector<std::pair<int, int>>();

    for (auto kv : tree.vertex()) {
        auto vertice = kv.first;
        auto weight = kv.second;

        if (tree.is_detatched(vertice)) {
            auto p = std::make_pair(weight, vertice);
            detatched_ids.push_back(p);
        } else {
            auto parent = tree.parent(vertice);
            auto cost = (double) tree.edge_weight(vertice, parent) / weight;
            auto p = std::make_pair(cost, vertice);
            nodes.push_back(p);
        }
    }

    std::sort(nodes.begin(), nodes.end());
    std::sort(detatched_ids.rbegin(), detatched_ids.rend());
    
    auto total_weight = tree.total_vertex_weight();
    auto ideal_weight_dist = (double)total_weight / n_partitions;
    auto max_weight = ceil(ideal_weight_dist * 1.1);
    auto min_weight = floor(ideal_weight_dist * 0.9);

    auto used_ids = std::unordered_set<int>();
    for (auto& partition : partitions) {
        for (auto& kv : nodes) {
            auto node = kv.second;
            if (used_ids.find(node) != used_ids.end()) {
                continue;
            }

            if (partition.weight() + tree.vertice_weight(node) < max_weight) {
                for (auto id : tree.ids_in_node(node)) {
                    partition.insert(id);
                }
                partition.increase_weight(tree.vertice_weight(node));
                used_ids.insert(node);
                tree.unpropagate_weight(node);
            }
        }
    }

    for (auto partition : partitions) {
        for (auto kv : detatched_ids) {
            auto id = kv.second;
            if (used_ids.find(id) != used_ids.end()) {
                continue;
            }
            if (partition.weight() + tree.vertice_weight(id) < max_weight) {
                partition.insert(id, tree.vertice_weight(id));
                used_ids.insert(id);
            }
        }
    }

    return partitions;
}

}