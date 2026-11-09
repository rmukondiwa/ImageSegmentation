#include "visualize.hpp"

namespace viz{
namespace{
    bool is_fg(const SegResult& seg, int x, int y)
    {
        return seg.fg[static_cast<size_t>(y)*seg.width +x];
    }

    // A pixel is on the boundary if any 4-neighbor has the opposite label.
    bool on_boundary(const SegResult& seg, int x, int y)
    {
        bool center = is_fg(seg, x, y);
        const int dx[] = {1, -1, 0, 0};
        const int dy[] = {0, 0, 1, -1};
        for(int k = 0; k<4; ++k)
        {
            int neighbor_x = x + dx[k], neighbor_y = y +dy[k];
            if(neighbor_x < 0 || neighbor_x >= seg.width || neighbor_y<0 || neighbor_y >= seg.height) continue;
            if(is_fg(seg, neighbor_x, neighbor_y) != center) return true;
        }

        return false;
    }
} // namespace

    Image seed_overlay(const Image& originalImg, const std::vector<Seed>& seeds)
    {
        Image out = originalImg;
        for(const Seed& s : seeds)
        {
            if(s.x<0 || s.x>= out.width || s.y <0 || s.y>=out.height) continue;

            // paint a small 3x3 blob so single-pixel seeds are visible

            for(int dy = -1; dy<=1; ++dy)
            {
                for(int dx = -1; dx<=1; ++dx)
                {
                    int x = s.x + dx, y = s.y+dy;
                    
                    if(x<0 || x>= out.width || y < 0 || y>= out.height) continue;
                    if(s.label ==1)
                    {
                        out.set(x, y, 255, 40, 40);     // fg = red
                    }
                    else
                    {
                        out.set(x,y,40,90,255);         // bg = blue
                    }
                }
            }

            return out;
        }
    }

    Image mask_image(const SegResult& seg)
    {
        Image out(seg.width, seg.height);
        for(int y = 0; y<seg.height; ++y)
        {
            for(int x = 0; x<seg.width; ++x)
            {
                uint8_t v = is_fg(seg, x, y) ? 255 : 0;
                out.set(x,y,v,v,v);
            }
        }

        return out;
    }

    Image boundary_overlay(const Image& originalImg, const SegResult& seg)
    {
        Image out = originalImg;
        for(int y = 0; y< seg.height; ++y)
        {
            for(int x = 0; x<seg.width; ++x)
            {
                if(on_boundary(seg, x, y))
                {
                    // Bright yellow-green contour, 2px (paint this pixel + right/down)
                    out.set(x,y,60,255,90);
                    if(x+1 < out.width) out.set(x+1, y, 60, 255, 90);
                }
            }
        }

        return out;
    }

    Image cutout(const Image& originalImg, const SegResult& seg)
    {
        Image out(originalImg.width, originalImg.height);

        const int cell = 12;    // checkerboard cell size
        for(int y = 0; y < originalImg.height; ++y)
        {
            for(int x = 0; x<originalImg.width; ++x)
            {
                if(is_fg(seg, x, y))
                {
                    out.set(x,y,originalImg.r(x,y), originalImg.g(x,y), originalImg.b(x,y));
                }
                else
                {
                    bool light = ((x/cell) + (y/cell)) %2 == 0;
                    uint8_t v = light ? 200 : 150;
                    out.set(x,y,v,v,v);
                }
            }
        }

        return out;
    }

    Image composite(const std::vector<Image>& panels)
    {
        if(panels.empty()) return Image{};

        int total_w = 0, max_h = 0;

        const int gap = 6;  // black separator between panels
        for(const Image&p : panels)
        {
            total_w += p.width;
            max_h = std::max(max_h, p.height);
        }

        total_w += gap * static_cast<int>(panels.size()-1);
        Image out(total_w, max_h);      // starts all-black( good separators/padding)
        int ox = 0;
        for(const Image& p : panels)
        {
            for(int y = 0; y<p.height; ++y)
            {
                for(int x = 0; x<p.width; ++x)
                {
                    out.set(ox+x, y, p.r(x,y), p.g(x,y), p.b(x,y));
                }
            }
            ox += p.width + gap;
        }

        return out;
    }

    Image sweep_montage(const Image& originalImg, const std::vector<Seed>& seeds, const std::vector<double>& lambdas, double sigma)
    {
        std::vector<Image> tiles;
        tiles.reserve(lambdas.size());
        for (double lam : lambdas) {
            BuildParams p;
            p.lambda = lam;
            p.sigma = sigma;
            SegResult seg = segment(originalImg, seeds, p);
            tiles.push_back(boundary_overlay(originalImg, seg));
        }
        if (tiles.empty()) return Image{};

        int cols = std::min<int>(3, static_cast<int>(tiles.size()));
        int rows = (static_cast<int>(tiles.size()) + cols - 1) / cols;
        const int gap = 6;
        int tw = originalImg.width, th = originalImg.height;
        Image out(cols * tw + (cols - 1) * gap, rows * th + (rows - 1) * gap);
        for (size_t i = 0; i < tiles.size(); ++i) {
            int c = static_cast<int>(i) % cols;
            int r = static_cast<int>(i) / cols;
            int ox = c * (tw + gap), oy = r * (th + gap);
            const Image& t = tiles[i];
            for (int y = 0; y < t.height; ++y)
            for (int x = 0; x < t.width; ++x)
                out.set(ox + x, oy + y, t.r(x, y), t.g(x, y), t.b(x, y));
        }
        return out;
    }

}
