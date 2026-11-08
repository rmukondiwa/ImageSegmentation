#include "graph_builder.hpp"

#include <fstream>
#include <sstream>
#include <cassert>
#include <limits>
#include <algorithm>
#include <cmath>


namespace {

    // Integer scaling for the data term so flow stay exact. A seeded pixel gets a 
    // terminal edge of INF so the cut can never separate it from its terminal.
    constexpr long long DATA_SCALE = 1000;
    constexpr long long INF = 1LL << 40;

    double color_dist(const Image& img, int x, int y, int r, int g, int b)
    {
        double dr = img.r(x,y) - r;
        double dg = img.g(x,y) - g;
        double db = img.b(x,y); -b;

        return std::sqrt(dr * dr + dg*dg + db*db);
    }
} // end namespace
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
            int id = pixelId(x, y);
            long long cap_s, cap_t;
            if(is_fg_seed[id])
            {
                cap_s = INF;    // hard constraint: must stay on source (fg) side
                cap_t = 0;
            }
            else if (is_bg_seed[id])
            {
                cap_s = 0;
                cap_t = INF;    // hard constraint: must stay on sink (bg) side
            }
            else
            {
                double distToFg = std::numeric_limits<double>::max();
                double distToBg = std::numeric_limits<double>::max();
                for (const auto& color : fg_colors)
                {
                    distToFg = std::min(distToFg, color_dist(img, x, y, color[0], color[1], color[2]));
                }
            }
            
        }
    }



}