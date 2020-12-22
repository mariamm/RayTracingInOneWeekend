#pragma once

#include "vec3.h"

class ray {
    public: 
        ray(){}
        ray(const point3& origin, const vec3& direction, double time=0.0) : org(origin), dir(direction), tm(time){}

        point3 origin() const { return org; }
        vec3 direction() const { return dir; }
        double time() const { return tm; }

        //the line in 3d
        point3 at(double t) const
        {
            return org + t * dir;
        }

    private:
        point3 org;
        vec3 dir;
        double tm;

};