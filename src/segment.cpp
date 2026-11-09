#include "segment.hpp"
#include <iostream>

SegResult segment(const Image& img, const std::vector<Seed>& seeds, 
                  const BuildParams& params)
{
    // construct graph
    BuiltGraph graph = build_graph(img, seeds, params);
    std::cout<< "built graph"<<std::endl;
    SegResult res;
    res.width = graph.width;
    res.height = graph.height;
    res.flow = graph.mf.max_flow(graph.s, graph.t);

    std::cout<< "we got max flow"<<std::endl;
    // Pixels still reachable from s in the residual graph are the foreground
    // (s-side of the min cut).
    std::vector<bool> side = graph.mf.min_cut_side(graph.s);
    res.fg.assign(static_cast<size_t>(res.width)*res.height, false);
    for(int i = 0; i<res.width*res.height; ++i)
    {
        res.fg[i] = side[i];
    }

    std::cout<< "we got res"<<std::endl;

    return res;
}