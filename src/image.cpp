#include "image.hpp"

#include <iostream>


Image load_image(const std::string& path)
{
    int w = 0, h=0, channels = 0;

    // Force 3 channels (RGB); stb handles the conversion
}