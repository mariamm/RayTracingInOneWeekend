#pragma once


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "external/stb-image/stb_image.h"
#include "external/stb-image/stb_image_write.h"
#include <math.h>
#include <limits>
#include <memory>
#include <cstdlib>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159265358979323846;

inline double clamp(double x, double minPoint, double maxPoint) {
    if (x < minPoint) return minPoint;
    if (x > maxPoint) return maxPoint;
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

inline double random_double(double minPoint, double maxPoint) {
    // Returns a random real in [min,max).
    return minPoint + (maxPoint - minPoint) * random_double();
}

inline int random_int(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}