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

}
