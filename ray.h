#pragma once

#include "vec3.h"

class Ray {
    public: 
        Ray(){}
        Ray(const Point3& origin, const Vec3& direction, double time=0.0) : org(origin), dir(direction), tm(time){}

        Point3 origin() const { return org; }
        Vec3 direction() const { return dir; }
        double time() const { return tm; }

        //the line in 3d
        Point3 at(double t) const
        {
            return org + t * dir;
        }

    private:
        Point3 org;
        Vec3 dir;
        double tm;

};