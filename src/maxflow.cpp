#include "maxflow.hpp"

#include <cassert>
#include <algorithm>
#include <queue>
#include <iostream>


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

//Push as much flow as possible from u to t along the level graph.
long long MaxFlow::dfs(int u, int t, long long pushed)
{
    if (u==t) return pushed;

    // dfs_iter[u] persists across calls within a phase so we never re-scan dead edges
    for(int& i = dfs_iter[u]; i< static_cast<int>(adjList[u].size()); ++i)
    {
        int id = adjList[u][i];
        Edge& e = edges[id];
        if(e.cap>0 && level[e.to] == level[u] +1)
        {
            long long got = dfs(e.to, t, std::min(pushed, e.cap));
            if(got >0)
            {
                e.cap -= got;
                edges[id^1].cap +=got;
                return got;
            }
        }
    }
    return 0;
}

long long MaxFlow::max_flow(int s, int t)
{
    assert(s != t);
    std::cout<< "about to start max flow"<<std::endl;
    long long flow = 0;
    int numOfLGraphs = 1;
    while(bfs(s, t))
    {
        std::cout<< "checking Level-Graph #" << numOfLGraphs<<std::endl;
        std::fill(dfs_iter.begin(), dfs_iter.end(), 0);
        while(true)
        {
            std::cout<< "u: " << s << "t: " << t<<std::endl;
            long long pushed = dfs(s, t, LLONG_MAX);
            if(pushed == 0) break;
            std::cout<< "pushed: " << pushed <<std::endl;
            flow += pushed; 
        }
    }

    return flow;
}

std::vector<bool> MaxFlow::min_cut_side(int s) const
{
    std::vector<bool> reachable(n, false);
    std::queue<int> q;
    reachable[s] = true;
    q.push(s);

    while(!q.empty())
    {
        int u = q.front();
        q.pop();

        for(int id : adjList[u])
        {
            const Edge& e = edges[id];
            if(e.cap > 0 && !reachable[e.to])
            {
                reachable[e.to] = true;
                q.push(e.to);
            }
        }
    }

    return reachable;
}