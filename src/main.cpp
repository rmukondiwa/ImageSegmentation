// CLI: input image, seed file, output dir, flags.
//  imageSegmentation <image> <seeds> <out_dir> [--sweep] [--lambda N] [--sigman N]
//     [--no-html]
#include <iostream>
#include <string>

#include "graph_builder.hpp"
#include "image.hpp"
#include "segment.hpp"
#include "visualize.hpp"

namespace {

std::string join(const std::string& dir, const std::string& name)
{
    if(dir.empty()) return name;
    if(dir.back() == '/') return dir + name;
    return dir + "/" + name;
}

// usage helper function prints the missing inputs as error
void usage(const char* prog)
{
    std::fprintf(stderr, 
                 "usage: %s <image> <seeds> <out_dir> "
                 "[--sweep] [--lambda N] [--sigma N] [--no-html]\n",
                prog);
}

} //namespace

int main(int argc, char** argv)
{

    // if user doesn't input min required inputs
    if(argc < 4)
    {
        usage(argv[0]);
        return 2;
    }

    std::string image_path = argv[1];
    std::string seeds_path = argv[2];
    std::string out_dir = argv[3];

    BuildParams params;
    bool do_sweep = false;
    bool write_html = true;

    for(int i = 4; i< argc; ++i)
    {
        std::string a = argv[i];

        if (a == "--sweep")
        {
            do_sweep = true;
        }
        else if (a == "--no-html")
        {
            write_html = false;
        }
        else if (a == "--lambda" && i +1 < argc) // if the user inputs their desired lamda and we have at least one input left
        {
            params.lambda = std::atof(argv[++i]);
        }
        else if (a == "--sigma" && i+1<argc) // if the user inputs their desired sigma and we have at least one input left
        {
            params.sigma = std::atof(argv[++i]);
        }
        else
        {
            std::fprintf(stderr, "unkown/incomplete arg: %s\n", a.c_str());
            usage(argv[0]);
            return 2;
        }
    }

    Image img = load_image(image_path);
    if (img.empty())
    {
        std::fprintf(stderr, "could not load image '%s'\n", image_path.c_str());
        return 1;
    }
    std::vector<Seed> seeds = load_seeds(seeds_path);
    if(seeds.empty())
    {
        std::fprintf(stderr, "no seeds loaded from '%s' (need 'x y label' lines)\n", seeds_path.c_str());
        return 1;
    }

    std::printf("image %dx%d, %zu seeds, lambda=%.2f sigma=%.2f\n", img.width,
                img.height, seeds.size(), params.lambda, params.sigma);

    // dinic's alg entry point
    SegResult seg = segment(img, seeds, params);
    std::printf("max-flow (min-cut capacity) = %lld\n", seg.flow);
    
    //Build and write artifacts.
    Image seeds_img = viz::seed_overlay(img, seeds);
    Image mask_img = viz::mask_image(seg);
    Image boundary_img = viz::boundary_overlay(img, seg);
    Image cutout_img = viz::cutout(img, seg);


}