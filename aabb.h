#pragma once

#include "vec3.h"
#include "ray.h"

// axis alligned bounding box

class aabb
{
public:
    aabb()
    {
        minPoint = Point3(0, 0, 0);
        maxPoint = Point3(0, 0, 0);
    }

    aabb(const Point3 &min_, const Point3 &max_) : minPoint(min_), maxPoint(max_){}

    Point3 minimum() const
    {
        return minPoint;
    }
    Point3 maximum() const
    {
        return maxPoint;
    }
    bool hit(const Ray& ray, double t_min, double t_max) const
    {
        /*
        t0=min( (x0−Ax)/bx, (x1−Ax) /bx)
        t1=max((x0−Ax)/ bx, (x1−Ax) / bx)
        */
        for (int i = 0; i < 3; i++)
        {   
            auto x0_Ax = minPoint[i] - ray.origin()[i];
            auto x1_Ax = maxPoint[i] - ray.origin()[i];
            auto bx = ray.direction()[i];
            double t0 = std::min( x0_Ax / bx, x1_Ax / bx );
            double t1 = std::max(x0_Ax / bx, x1_Ax / bx);

            //if(t0 < t_min || t1 > t_max)
            t_min = std::min(t_min, t0);
            t_max = std::max(t_max, t1);
            if (t_max <= t_min)
                return false;
        }
        return true;
    }
    void surround(const aabb& other)
    {
        for(int i = 0; i<3; i++)
        {
            maxPoint[i] = std::min(minPoint[i], other.maximum()[i]);
            maxPoint[i] = std::max(maxPoint[i], other.maximum()[i]); 
        }
    }

private:
    Point3 minPoint, maxPoint;

};