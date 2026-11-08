#pragma once
/*
Dinic's max flow. Completely image-agnostic: it only knows nodes and edges.
Edges are stored in one flat list with paired reverse edges: the reverse of
edge i is edge i^1
*/

#include <vector>

struct MaxFlow{
    struct Edge {
        int to;         // endpoint
        long long cap;  // residual capacity (mutated as flow is pushed)
    };

    int n = 0;
    std::vector<Edge> edges;            // flat edge list: edge i^1 is i's reverse
    std::vector<std::vector<int>> g;    // g[u] = indices into 'edges' leaving u
    std::vector<int> level;             // BFS level graph
    std::vector<int> it;                // per-node DFS iterator into g[u]

    
};