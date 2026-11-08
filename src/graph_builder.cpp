#include "graph_builder.hpp"

#include <fstream>

std::vector<Seed> load_seeds(const std::string& path)
{
    std::vector<Seed> seeds;
    std::ifstream in(path);

    if (!in)
    {
        std::fprintf(stderr, "load_seeds: cannot open '%s'\n", path.c_str());
        return seeds;
    }
}