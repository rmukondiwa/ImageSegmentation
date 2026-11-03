#pragma once
// Thin RGB image wrapper over std_image / stb_image_write. Always 3 channels
// (we drop alpha on load) so pixel math downstream stays simple.

#include <cstdint>
#include <string>
#include <vector>

struct Image {
    int width = 0;
    int height = 0;
    std::vector<uint8_t> data; // row-major, 3 bytes (RGB) per pixel

    Image() = default;
    Image(int w, int h) : width(w), height(h), data(static_cast<size_t>(w) * h * 3, 0) {}

    bool empty() const {return width <= 0 || height <= 0;}
    size_t idx(int x, int y) const { return (static_cast<size_t>(y)*width+x)*3;}

    uint8_t r(int x, int y) const { return data[idx(x,y) +0]; }
    uint8_t g(int x, int y) const { return data[idx(x,y)+1]; }
    uint8_t b(int x, int y) const { return data[idx(x,y)+2]; }

}