#pragma once
#include <vector>

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