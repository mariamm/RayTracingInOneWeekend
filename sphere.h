#pragma once

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class Material;

class Sphere : public Hittable
{
    public:
        Sphere() {
            center = Point3(0, 0, 0);
            radius = 0.0;
        }
        Sphere(Point3 _center, double _radius, std::shared_ptr<Material> _material) :center(_center), radius(_radius), material(_material) {}

        virtual bool hit(const Ray& r, const double& min_t, const double& max_t, hit_record& hitrecord) const override;
        static void get_uv_coordinates(const Point3 &p, double& u, double& v);

        virtual bool boundingBox(double time0, double time1, aabb& output_box) const override;

    private:
        Point3 center;
        double radius;
        std::shared_ptr<Material> material;
};


bool Sphere::hit(const Ray& r, const double& t_min, const double& t_max, hit_record& rec) const
{
    Vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_uv_coordinates(outward_normal, rec.u, rec.v);
    rec.material_ptr = material;

    return true;
}

void Sphere::get_uv_coordinates(const Point3& p, double& u, double& v)
{
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    double theta = acos(-p.y());
    double phi = atan2(-p.z(), p.x()) + pi;

    //assert(!isnan(theta));
    //assert(!isnan(phi));

    u = phi / (2 * pi);
    v = theta / pi;
}

bool Sphere::boundingBox(double time0, double time1, aabb& output_box) const
{
    return false;
}