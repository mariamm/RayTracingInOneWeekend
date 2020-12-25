#pragma once


#define STB_IMAGE_IMPLEMENTATION
#include "external/stb-image/stb_image.h"
#include <math.h>
#include <limits>
#include <memory>
#include <cstdlib>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159265358979323846;

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline double degreeToRadians(double degree)
{
    return degree * pi / 180.;
}

inline double radiansToDegree(double radian)
{
    return 180. * radian / pi;
}

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}
