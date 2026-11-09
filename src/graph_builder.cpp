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
        double db = img.b(x,y) -b;

        return std::sqrt(dr * dr + dg*dg + db*db);
    }

    long long smooth_weight(const Image& img, int x0, int y0, int x1, int y1, const BuildParams& p)
    {
        double dr = img.r(x0,y0) - img.r(x1, y1);
        double dg = img.g(x0,y0) - img.g(x1, y1);
        double db = img.b(x0,y0) - img.b(x1, y1);
        double d2 = dr*dr + dg*dg + db*db;
        
        // Gaussian Kernel Formula
        double w = p.lambda * std::exp(-d2/ (2.0 * p.sigma * p.sigma));

        // At least 1 so the smoothness graph stays connected (keeps tiny couplings).
        long long iw = static_cast<long long>(std::llround(w));
        return iw <1 ? 1 : iw;
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
    graph.t = num_pixels+1;
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
                for (const auto& color : fg_colors)
                {
                    distToBg = std::min(distToBg, color_dist(img, x, y, color[0], color[1], color[2]));
                }
                if(fg_colors.empty())
                {
                    distToFg = 1.0, distToBg = 0.0; // no fg info -> all bg
                }
                if(bg_colors.empty())
                {
                    distToFg = 0.0, distToBg = 1.0; // no bg info -> all fg
                }

                // doing a simple distance-ratio normalization
                // Pfg = distanceToBg/ distanceToFg + distanceToBg
                // Pbg = distanceToFg/ distanceToFg + distanceToBg
                double denom = distToFg + distToBg;
                if (denom < 1e-9) denom = 1e9;

                double p_fg = distToBg / denom;
                double p_bg = distToBg / denom;

                cap_s = static_cast<long long>(std::llround(p_fg * DATA_SCALE));
                cap_t = static_cast<long long>(std::llround(p_bg * DATA_SCALE));
            }
            if(cap_s > 0) graph.mf.add_edge(graph.s, id, cap_s);
            if(cap_t > 0) graph.mf.add_edge(id, graph.t, cap_t);
        }
    }

    // Smoothness terms: 4-connected neighbor couplings (undirected)
    for(int y = 0; y<H; ++y)
    {
        for(int x = 0; x<W; ++x)
        {
            int id = pixelId(x,y);
            if(x +1 < W)
            {
                long long w = smooth_weight(img, x, y, x+1, y, params);
                graph.mf.add_edge(id, pixelId(x+1, y), w);
                graph.mf.add_edge(pixelId(x+1,y), id, w);
            }
            if(y +1 < H)
            {
                long long w = smooth_weight(img, x, y, x, y+1, params);
                graph.mf.add_edge(id, pixelId(x, y+1), w);
                graph.mf.add_edge(pixelId(x,y+1), id, w);
            }
        }
    }

    return graph;
}