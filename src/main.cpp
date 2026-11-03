// CLI: input image, seed file, output dir, flags.
//  imageSegmentation <image> <seeds> <out_dir> [--sweep] [--lambda N] [--sigman N]
//     [--no-html]
#include <iostream>
#include <string>

#include "graph_builder.hpp"

// usage helper function prints the missing inputs as error
void usage(const char* prog)
{
    std::fprintf(stderr, 
                 "usage: %s <image> <seeds> <out_dir> "
                 "[--sweep] [--lambda N] [--sigma N] [--no-html]\n",
                prog);
}

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



}