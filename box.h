#pragma once

#include "hittable.h"
#include "hittable_list.h"
#include "axis_rectangle.h"

class Box : public Hittable
{
public:

    Box()
    {
        minimum = Point3();
        maximum = Point3();
    }
    Box(shared_ptr<Material> m, const Point3& min_, const Point3& max_) :  minimum(min_), maximum(max_)
    {
        sides.add(make_shared<Rect_xy>(m, min_.x(), max_.x(), min_.y(), max_.y(), min_.z()));
        sides.add(make_shared<Rect_xy>(m, min_.x(), max_.x(), min_.y(), max_.y(), max_.z()));

        sides.add(make_shared<Rect_xz>(m, min_.x(), max_.x(), min_.z(), max_.z(), min_.y()));
        sides.add(make_shared<Rect_xz>(m, min_.x(), max_.x(), min_.z(), max_.z(), max_.y()));

        sides.add(make_shared<Rect_yz>(m, min_.y(), max_.y(), min_.z(), max_.z(), min_.x()));
        sides.add(make_shared<Rect_yz>(m, min_.y(), max_.y(), min_.z(), max_.z(), max_.x()));
    }

    virtual bool hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const override;
    virtual bool boundingBox(double time0, double time1, aabb& output_box) const override;

private:
    shared_ptr<Material> material;
    Point3 minimum;
    Point3 maximum;
    HittableList sides;
};


bool Box::hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const
{
    return sides.hit(r, min_t, max_t, hitrecord);
}
bool Box::boundingBox(double time0, double time1, aabb& output_box) const
{
    output_box = aabb(minimum, maximum);
    return true;
}