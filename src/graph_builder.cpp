#include "graph_builder.hpp"

#include <fstream>
#include <sstream>
#include <csassert>

std::vector<Seed> load_seeds(const std::string& path)
{
    std::vector<Seed> seeds;
    std::ifstream in(path);

    if (!in)
    {
        std::fprintf(stderr, "load_seeds: cannot open '%s'\n", path.c_str());
        return seeds;
    }
    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        Seed s{};
        if(ss >> s.x >> s.y >> s.label)
        {
            seeds.push_back(s);
        }
    }

    return seeds; // [ Seed{}, Seed{},...]
}

BuiltGraph build_graph(const Image& img, const std::vector<Seed>& seeds, const BuildParams& params)
{
    assert(!img.empty());
    BuiltGraph graph;
    graph.width = img.width;
    graph.height = img.height;
    const int W = img.width, H = img.height;
    const int num_pixels = W * H;
    graph.s = num_pixels;
    graph.t = num_pixels;
    
    


}