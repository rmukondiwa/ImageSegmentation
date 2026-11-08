#include "maxflow.hpp"

#include <cassert>
#include <algorithm>
#include <queue>


void MaxFlow::initializeGraph(int num_nodes)
{
    n = num_nodes;
    edges.clear();
    adjList.assign(n, {});
    level.assign(n, -1);
    dfs_iter.assign(n,0);
}

void MaxFlow::add_edge(int u, int v, long long cap)
{
    adjList[u].push_back(static_cast<int>(edges.size()));
    edges.push_back({v, cap});
    adjList[v].push_back(static_cast<int>(edges.size()));
    edges.push_back({u, 0});    // reverse edge starts saturated
}

// bfs from source to sink for all untraversed pathways
bool MaxFlow::bfs(int s, int t)
{
    std::fill(level.begin(), level.end(), -1);
    std::queue<int> q;
    q.push(s);
    level[s] = 0;

    while(!q.empty())
    {
        int u = q.front();
        q.pop();
        for(int id : adjList[u])
        {
            const Edge& e = edges[id];
            if(e.cap > 0 && level[e.to]<0)
            {
                level[e.to] = level[u] +1;
                q.push(e.to);
            }
        }
    }

    return level[t] >=0;
}

long long MaxFlow::max_flow(int s, int t)
{
    assert(s != t);
    long long flow = 0;

    while(bfs(s, t))
    {

    }

}