#include "min_cut.h"

namespace model {

// Used by refennel to call refennel if it's the first time it partitions
bool first_repartition = true;

workload::PartitionManager multilevel_cut(
    Graph& graph, idx_t n_partitions, CutMethod cut_method
) {
    auto& vertex = graph.vertex();
    idx_t n_vertice = vertex.size();
    idx_t n_edges = n_vertice * (n_vertice - 1);
    idx_t n_constrains = 1;

    auto vertice_weight = std::vector<idx_t>();
    for (auto i = 0; i < vertex.size(); i++) {
        vertice_weight.push_back(vertex.at(i));
    }

    auto x_edges = std::vector<idx_t>();
    auto edges = std::vector<idx_t>();
    auto edges_weight = std::vector<idx_t>();

    x_edges.push_back(0);
    for (auto vertice = 0; vertice < vertex.size(); vertice++) {
        auto last_edge_index = x_edges.back();
        auto n_neighbours = graph.vertice_edges(vertice).size();
        x_edges.push_back(last_edge_index + n_neighbours);

        for (auto& vk: graph.vertice_edges(vertice)) {
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
    options[METIS_OPTION_UFACTOR] = 200;

    idx_t objval;
    auto vertex_partitions = std::vector<idx_t>(n_vertice, 0);
    if (cut_method == METIS) {
        METIS_PartGraphKway(
            &n_vertice, &n_constrains, x_edges.data(), edges.data(),
            vertice_weight.data(), NULL, edges_weight.data(), &n_partitions, NULL,
            NULL, options, &objval, vertex_partitions.data()
        );
    } else {
        double imbalance = 0.2;  // equal to METIS default imbalance
        kaffpa(
            &n_vertice, vertice_weight.data(), x_edges.data(),
            edges_weight.data(), edges.data(), &n_partitions,
            &imbalance, true, -1, FAST, &objval,
            vertex_partitions.data()
        );
    }

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

    return workload::PartitionManager(partitions);
}

int fennel_inter_cost(
    const std::unordered_map<int, int>& edges,
    const workload::Partition& partition
) {
    auto& vertex = partition.data();
    auto cost = 0;
    for (auto& kv : edges) {
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
    Graph& graph, int vertice, const std::vector<workload::Partition>& partitions,
    int max_partition_size, double alpha, double gamma
) {
    double biggest_score = -DBL_MAX;
    auto id = 0;
    auto designated_partition = 0;
    for (auto& partition : partitions) {
        auto partition_weight = partition.weight();
        if (max_partition_size > 0) {
            if (partition_weight + graph.vertice_weight(vertice) > max_partition_size) {
                id++;
                continue;
            }
        }

        auto& edges = graph.vertice_edges(vertice);

        auto inter_cost = fennel_inter_cost(edges, partition);
        auto intra_cost =
            (std::pow(partition.weight() + graph.vertice_weight(vertice), gamma));
        intra_cost -= std::pow(partition.weight(), gamma);
        intra_cost *= alpha;
        auto score = inter_cost - intra_cost;

        if (score > biggest_score) {
            biggest_score = score;
            designated_partition = id;
        }
        id++;
    }

    return designated_partition;
}

workload::PartitionManager fennel_cut(Graph& graph, int n_partitions) {
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
    auto partition_max_size = 1.2 * graph.total_vertex_weight() / n_partitions;
    for (auto& kv: graph.vertex()) {
        auto vertice = kv.first;
        auto partition = fennel_vertice_partition(
            graph, vertice, partitions,
            partition_max_size, alpha, gamma
        );
        if (partition == -1) {
            partition_max_size = 0;
            partition = fennel_vertice_partition(
                graph, vertice, partitions,
                partition_max_size, alpha, gamma
            );
        }
        partitions[partition].insert(vertice, graph.vertice_weight(vertice));
    }

    return workload::PartitionManager(partitions);
}

workload::PartitionManager refennel_cut(
    Graph& graph, workload::PartitionManager& partition_scheme
) {
    if (first_repartition) {
        first_repartition = false;
        return fennel_cut(graph, partition_scheme.n_partitions());
    }
    const auto edges_weight = graph.total_edges_weight();
    const auto vertex_weight = graph.total_vertex_weight();
    const auto gamma = 3 / 2.0;
    const auto alpha =
        edges_weight * std::pow(partition_scheme.n_partitions(), (gamma - 1)) / std::pow(graph.total_vertex_weight(), gamma);
    auto partition_max_size = 1.2 * graph.total_vertex_weight() / partition_scheme.n_partitions();
    for (auto& kv: graph.vertex()) {
        auto vertice = kv.first;
        auto new_partition = fennel_vertice_partition(
            graph, vertice, partition_scheme.partitions(),
            partition_max_size, alpha, gamma
        );
        if (new_partition == -1) {
            partition_max_size = 0;
            new_partition = fennel_vertice_partition(
                graph, vertice, partition_scheme.partitions(),
                partition_max_size, alpha, gamma
            );
        }
        auto old_partition = partition_scheme.value_to_partition(vertice);
        partition_scheme.remove_value(vertice);
        partition_scheme.add_value(vertice, new_partition, graph.vertice_weight(vertice));
    }

    return partition_scheme;

}

workload::PartitionManager spanning_tree_cut(SpanningTree tree, int n_partitions) {
    auto partitions = std::vector<workload::Partition>();
    for (auto i = 0; i < n_partitions-1; i++) {
        auto partition = workload::Partition();
        partitions.push_back(partition);
    }

    auto detatched_ids = std::vector<std::pair<double, int>>();
    auto nodes = std::vector<std::pair<int, int>>();

    for (auto& kv : tree.vertex()) {
        auto vertice = kv.first;
        auto weight = kv.second;

        if (tree.is_detatched(vertice)) {
            auto p = std::make_pair(weight, vertice);
            detatched_ids.push_back(p);
        } else if (not tree.is_inside_node(vertice)) {
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
                partition.increase_weight(node, tree.vertice_weight(node));
                used_ids.insert(node);
                tree.unpropagate_weight(node);
            }
            if (partition.weight() >= min_weight) {
                break;
            }
        }
    }
    for (auto& partition : partitions) {
        for (auto& kv : detatched_ids) {
            auto id = kv.second;
            if (used_ids.find(id) != used_ids.end()) {
                continue;
            }
            if (partition.weight() + tree.vertice_weight(id) < max_weight) {
                partition.insert(id, tree.vertice_weight(id));
                used_ids.insert(id);
            }
            if (partition.weight() >= min_weight) {
                break;
            }
        }
    }

    auto final_partition = workload::Partition();
    for (auto& kv : nodes) {
        auto node = kv.second;
        if (used_ids.find(node) != used_ids.end()) {
            continue;
        }

        for (auto id : tree.ids_in_node(node)) {
            final_partition.insert(id);
        }
        final_partition.increase_weight(node, tree.vertice_weight(node));
        used_ids.insert(node);
        tree.unpropagate_weight(node);
    }
    for (auto& kv : detatched_ids) {
        auto id = kv.second;
        if (used_ids.find(id) != used_ids.end()) {
            continue;
        }
        final_partition.insert(id, tree.vertice_weight(id));
        used_ids.insert(id);
    }

    partitions.push_back(std::move(final_partition));
    return partitions;
}

}
