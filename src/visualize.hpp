#pragma once

#include "image.hpp"
#include "graph_builder.hpp"

namespace viz {

    // Original with fg seeds painted red and bg seeds blue (the *input* to the cut).
    Image seed_overlay(const Image& originalImg, const std::vector<Seed>& seeds);
}