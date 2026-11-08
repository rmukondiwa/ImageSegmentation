#pragma once
/*
    Ties the pieces together: build graph -> run max-flow -> extract a boolean
    foreground mask from the min cut.
*/

#include <vector>

#include "image.hpp"
#include "graph_builder.hpp"

struct SegResult{
    int width = 0;
    int height = 0;
    std::vector<bool> fg;   // fg[y*width+x] == true => foreground
    long long flow = 0;     // max-flow value (== min-cut capacity)
};

SegResult segment(const Image& img, const std::vector<Seed>& seeds, const BuildParams& params);