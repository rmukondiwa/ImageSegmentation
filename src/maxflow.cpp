#include "maxflow.hpp"


void MaxFlow::initializeGraph(int num_nodes)
{
    n = num_nodes;
    edges.clear();
    adjList.assign(n, {});
    level.assign(n, -1);
    it.assign(n,0);
}

void MaxFlow::add_edge(int u, int v, long long cap)
{
    adjList[u].push_back(static_cast<int>(edges.size()));
    edges.push_back({v, cap});
    adjList[v].push_back(static_cast<int>(edges.size()));
    edges.push_back({u, 0});    // reverse edge starts saturated
}

long long MaxFlow::max_flow(int s, int t)
{
    
}