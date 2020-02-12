#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>

#include "spanning_tree.h"

using namespace std;
// { (a,d) (a,b) (c,e) (c,g) (d,f) (g,h) (a,c) (b,d) (e,g) (c,d) (c,h) (d,h) (f,h)}
int main() {
    auto spanning_tree = model::SpanningTree();
    auto edges = vector<pair<int, int>>();
    edges.push_back(make_pair(0, 3));  // (a, d)
    edges.push_back(make_pair(0, 1));  // (a, b)
    edges.push_back(make_pair(2, 4));  // (c, e)
    edges.push_back(make_pair(2, 6));  // (c, g)
    edges.push_back(make_pair(3, 5));  // (d, f)
    edges.push_back(make_pair(6, 7));  // (g, h)
    edges.push_back(make_pair(0, 2));  // (a, c)
    edges.push_back(make_pair(1, 3));  // (b, d)
    edges.push_back(make_pair(4, 6));  // (e, g)
    edges.push_back(make_pair(2, 3));  // (c, d)
    edges.push_back(make_pair(2, 7));  // (c, h)
    edges.push_back(make_pair(3, 7));  // (d, h)
    edges.push_back(make_pair(5, 7));  // (f, h)

    std::unordered_map<int, char> p({
        {0, 'a'},
        {1, 'b'},
        {2, 'c'},
        {3, 'd'},
        {4, 'e'},
        {5, 'f'},
        {6, 'g'},
        {7, 'h'},

    });

    for (auto edge: edges) {
        spanning_tree.add_edge(edge);
    }

    auto map = spanning_tree.vertice_to_id();
    for (auto kv : map) {
        cout << "Node " << p[kv.first] << " is in supernode " << p[kv.second] << endl;
    }

    spanning_tree.print_stuff();

}