#include "visualize.hpp"

namespace viz{


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

        Image mask_image(const SegResult& seg)
        {
            
        }

    }
}