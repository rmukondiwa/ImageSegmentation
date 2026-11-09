#pragma once

#include "image.hpp"
#include "graph_builder.hpp"
#include "segment.hpp"

namespace viz {

    // Original with fg seeds painted red and bg seeds blue (the *input* to the cut).
    Image seed_overlay(const Image& originalImg, const std::vector<Seed>& seeds);

    // White foreground, black background
    Image mask_image(const SegResult& seg);

    // The hero shot: bright contour along the mask boundary, over the original.
    Image boundary_overlay(const Image& originalImg, const SegResult& seg);
}