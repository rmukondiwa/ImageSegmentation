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
    std::vector<std::vector<int>> adjList;    // g[u] = indices into 'edges' leaving u
    std::vector<int> level;             // BFS level graph
    std::vector<int> dfs_iter;              // per-node DFS iterator into g[u]

    void initializeGraph(int num_nodes);

    // Adds forward edge u->v with capacity 'cap' and a reverse v->u with capacity 0
    void add_edge(int u, int v, long long cap);

    long long max_flow(int s, int t);

    private:
        bool bfs(int s, int t);
        long long dfs(int u, int t, long long pushed);
   };