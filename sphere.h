#pragma once

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class Material;

class Sphere : public hittable
{
    public:
        Sphere() {
            center = point3(0, 0, 0);
            radius = 0.0;
        }
        Sphere(point3 _center, double _radius, std::shared_ptr<Material> _material) :center(_center), radius(_radius), material(_material) {}

        virtual bool hit(const ray& r, const double& min_t, const double& max_t, hit_record& hitrecord) const override;
    private:
        point3 center;
        double radius;
        std::shared_ptr<Material> material;
};


bool Sphere::hit(const ray& r, const double& t_min, const double& t_max, hit_record& rec) const
{
    vec3 oc = r.origin() - center;
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
    rec.normal = (rec.p - center) / radius;
    rec.material_ptr = material;

    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}