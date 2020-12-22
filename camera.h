#pragma once

#include "vec3.h"
#include "ray.h"
class Camera {

    public:
        Camera(point3 lookfrom, point3 lookat, vec3 vup,
            double vfov_degrees, double aspectRatio, double focalDist, double aperature,
            double time0_ = 0, double time1_ = 0) : time0(time0_), time1(time1_)
        {
            lens_radius = aperature / 2.;

            double theta = degreeToRadians(vfov_degrees);
            auto h = tan(theta / 2);
            origin = lookfrom;
            w = unit_vector(lookfrom - lookat); //lookat vector
            u = unit_vector(cross(vup, w)); //right vector (left axis)
            v = cross(w, u);                //up vector

            //virtual viewport through which to pass our scene rays, We'll just pick a viewport two units in height
            double viewportHeight = 2.0 * h;
            double viewportWidth = aspectRatio * viewportHeight;

            horizontal = focalDist * viewportWidth * u; //virtual plane horizontal axis
            vertical = focalDist * viewportHeight * v; //virtual plane vertical axis
            lower_left_corner = origin - horizontal / 2. - vertical / 2. - focalDist*w;
        }

        ray get_ray(double s, double t) const
        {
            vec3 lens_disk = lens_radius * random_in_unit_disk();
            vec3 offset = u * lens_disk.x() + v * lens_disk.y();
            return ray( origin+offset, 
                        lower_left_corner + s * horizontal + t * vertical - origin - offset, 
                        random_double(time0, time1));
        }
    private:
            point3 origin;
            vec3 horizontal;
            vec3 vertical;
            vec3 lower_left_corner;
            vec3 u, v, w;
            double lens_radius;
            double time0, time1; // shutter open/close times
};