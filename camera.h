#pragma once

#include "vec3.h"
#include "ray.h"
class Camera {

    public:
        Camera(Point3 lookfrom, Point3 lookat, Vec3 vup,
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

        Ray get_ray(double s, double t) const
        {
            Vec3 lens_disk = lens_radius * random_in_unit_disk();
            Vec3 offset = u * lens_disk.x() + v * lens_disk.y();
            return Ray( origin+offset, 
                        lower_left_corner + s * horizontal + t * vertical - origin - offset, 
                        random_double(time0, time1));
        }
    private:
            Point3 origin;
            Vec3 horizontal;
            Vec3 vertical;
            Vec3 lower_left_corner;
            Vec3 u, v, w;
            double lens_radius;
            double time0, time1; // shutter open/close times
};