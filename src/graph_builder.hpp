#pragma once
#include <vector>

#include "maxflow.hpp"
#include "image.hpp"

// label: 1 = foreground seed, 0 = background seed
struct Seed {
    int x;
    int y;
    int label;
};

// Seed file format: one "x y label" per line. Lines starting with '#' and blank
// lines are ignored
std::vector<Seed> load_seeds(const std::string& path);

struct BuildParams {
    double lambda = 50.0;
    double sigma = 30.0;
};

struct BuiltGraph {
    MaxFlow mf;
    int s = 0;  // source node id (foreground terminal)
    int t = 0;  // sink node id (background terminal)
    int width = 0;
    int height = 0;
};

// Node id for pixel (x,y) is y* width + x; s and t are the two extra ids.
BuiltGraph build_graph(const Image& img, const std::vector<Seed>& seeds, 
                  const BuildParams& params);