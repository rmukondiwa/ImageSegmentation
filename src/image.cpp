#include "image.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third_party/stb_image_write.h"

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

    Image img(w, h);
    img.data.assign(raw, raw + static_cast<size_t>(w)*h*3);
    stbi_image_free(raw);
    return img;
}

bool save_png(const std::string& path, const Image& img)
{
    if (img.empty())
    {
        std::fprintf(stderr, "save.png: refusing to write empty image '%s'\n", 
                    path.c_str());
        return false;
    }

    int ok = stbi_write_png(path.c_str(), img.width, img.height, 3,
                            img.data.data(), img.width*3);
    if (!ok)
    {
        std::fprintf(stderr, "save_png: failed to write '%s'\n", path.c_str());
        return false;
    }
    return true;
}