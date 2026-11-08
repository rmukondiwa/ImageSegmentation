#pragma once
/*
    Ties the pieces together: build graph -> run max-flow -> extract a boolean
    foreground mask from the min cut.
*/

#include <vector>


struct SegResult{
    int width = 0;
    int height = 0;
    std::vector<bool> fg;   // fg[y*width+x] == true => foreground
    long long flow = 0;     // max-flow value (== min-cut capacity)
}