#include "image.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"


Image load_image(const std::string& path)
{
    int w = 0, h=0, channels = 0;

    // Force 3 channels (RGB); stb handles the conversion
    uint8_t* raw = stbi_load(path.c_str(), &w, &h, &channels, 3);
    if(!raw)
    {
        std::fprintf(stderr, "load_image: failed to load '%s': %s\n",
                     path.c_str(), stbi_failure_reason());
        return Image{};
    }

}