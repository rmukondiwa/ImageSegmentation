// CLI: input image, seed file, output dir, flags.
//  imageSegmentation <image> <seeds> <out_dir> [--sweep] [--lambda N] [--sigman N]
//     [--no-html]
#include <iostream>
#include <string>

#include "graph_builder.hpp"
#include "image.hpp"

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
            std::fprintf(stderr, "unkown/incomplete arg: %s\n", std::to_string(a));
            usage(argv[0]);
            return 2;
        }
    }

    Image img = load_image(image_path);



}