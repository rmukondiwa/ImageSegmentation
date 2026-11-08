#include "graph_builder.hpp"

#include <fstream>
#include <sstream>
#include <cassert>

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
    graph.mf.initializeGraph(num_pixels+2);     // the +2 for the source and sink nodes

    auto pixelId = [W](int x, int y){
        return y * W + x;
    };

    // Collect seed colors for the (dead-simple) nearest-seed color model.
    std::vector<std::array<int, 3>> fg_colors, bg_colors;
    std::vector<bool> is_fg_seed(num_pixels, false), is_bg_seed(num_pixels, false);
    for (const Seed& s : seeds)
    {
        if(s.x < 0 || s.x >= W || s.y < 0 || s.y >= H) continue;
        int id = pixelId(s.x, s.y);

        if(s.label == 1)
        {
            fg_colors.push_back({img.r(s.x, s.y), img.g(s.x, s.y), img.b(s.x, s.y)});
            is_fg_seed[id] = true;
        }
        else
        {
            bg_colors.push_back({img.r(s.x, s.y), img.g(s.x, s.y), img.b(s.x, s.y)});
            is_bg_seed[id] = true;
        }
    }

    // Data Terms: terminal edges s->p ("looks fg") and p->t ("looks bg");
    for (int y = 0; y<H; ++y)
    {
        for(int x = 0; x<W; ++x)
        {
            
        }
    }



}