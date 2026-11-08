#include "maxflow.hpp"


void MaxFlow::initializeGraph(int num_nodes)
{
    n = num_nodes;
    edges.clear();
    g.assign(n, {});
    level.assign(n, -1);
    it.assign(n,0);
}