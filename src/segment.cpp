#include "segment.hpp"

SegResult segment(const Image& img, const std::vector<Seed>& seeds, 
                  const BuildParams& params)
{
    // construct graph
    BuiltGraph graph = build_graph(img, seeds, params);
    SegResult res;
    res.width = graph.width;
    res.height = graph.height;
    res.flow = graph.mf.max_flow(graph.s, graph.t);

    // Pixels still reachable from s in the residual graph are the foreground
    // (s-side of the min cut).
    std::vector<bool> side = graph.mf.min_cut_side(graph.s);
}