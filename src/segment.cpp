#include "segment.hpp"

SegResult segment(const Image& img, const std::vector<Seed>& seeds, 
                  const BuildParams& params)
{
    // construct graph
    BuiltGraph graph = build_graph(img, seeds, params);
}