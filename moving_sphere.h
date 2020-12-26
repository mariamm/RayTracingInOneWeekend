#pragma once
#include "hittable.h"

using std::shared_ptr;
class MovingSphere : public Hittable
{
    public:
        MovingSphere()
        {
            t0 = 0.;
            t1 = 0.;
            c0 = Point3(0,0,0);
            c1 = Point3(0, 0, 0);
            radius = 0;
        }

        MovingSphere(double time_0, double time_1, Point3 center_0, Point3 center_1, double radius_, shared_ptr<Material> material) 
            : t0(time_0), t1(time_1), c0(center_0), c1(center_1), radius(radius_), m(material){}

        virtual bool hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const override;
        virtual bool boundingBox(double time0, double time1, aabb& output_box) const override;

        Point3 centerAtTime(double time) const
        {
            return c0 + ((time - t0) / (t1 - t0)) * (c1 - c0);
        }
    private:
        double t0;
        double t1;
        Point3 c0;
        Point3 c1;
        double radius;
        shared_ptr<Material> m;
};

bool MovingSphere::hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const
{
    Point3 center = centerAtTime(r.time());
    Vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < min_t || max_t < root) {
        root = (-half_b + sqrtd) / a;
        if (root < min_t || max_t < root)
            return false;
    }

    hitrecord.t = root;
    hitrecord.p = r.at(hitrecord.t);
    hitrecord.normal = (hitrecord.p - center) / radius;
    hitrecord.material_ptr = m;

    Vec3 outward_normal = (hitrecord.p - center) / radius;
    hitrecord.set_face_normal(r, outward_normal);

    return true;
}

bool MovingSphere::boundingBox(double time0, double time1, aabb& output_box) const
{
    auto center0 = centerAtTime(t0);
    auto center1 = centerAtTime(t1);

    output_box = aabb();
    aabb box1 = aabb( center0- radius,  center0 + radius);
    output_box.surround(box1);

    aabb box2 = aabb(center1 - radius, center1 + radius);
    output_box.surround(box2);

    return true;
}