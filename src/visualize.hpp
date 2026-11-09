#pragma once

#include "image.hpp"

namespace viz {

    // Original with fg seeds painted red and bg seeds blue (the *input* to the cut).
    Image seed_overlay(const Image& orig, const std::vector<Seed>& seeds);
}