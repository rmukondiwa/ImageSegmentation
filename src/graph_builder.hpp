#pragma once


// label: 1 = foreground seed, 0 = background seed
struct Seed {
    int x;
    int y;
    int label;
};

struct BuildParams {
    double lambda = 50.0;
    double sigma = 30.0;
};