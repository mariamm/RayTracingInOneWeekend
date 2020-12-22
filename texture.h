#pragma once

#include "util.h"
#include "vec3.h"

class texture 
{
    public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture
{
    public:
        solid_color() { color_value = color(0, 0, 0); }

        solid_color(double r, double g, double b)
        {
            color_value = color(r, g, b);
        }

        virtual color value(double u, double v, const point3& p) const override
        {
            return color_value;
        }

    private:
        color color_value;
};