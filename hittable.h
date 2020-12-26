#pragma once

#include <memory>

#include "vec3.h"
#include "ray.h"
#include "util.h"
#include "aabb.h"

class Material;

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t; //hit point distance on the ray
    double u, v; //surface coordinates for texture
    std::shared_ptr<Material> material_ptr;
    bool front_face;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
    public:
        virtual bool hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const = 0;
        virtual bool boundingBox(double time0, double time1, aabb& output_box) const = 0;

};