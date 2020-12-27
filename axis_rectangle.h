#pragma once

#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

class Rect_xy : public Hittable
{
public:
    Rect_xy()
    {
        x0 = 0;
        x1 = 0;
        y0 = 0;
        y1 = 0;
    }

    Rect_xy(shared_ptr<Material> material_, double x_0, double x_1, double y_0, double y_1, double k_)
        : material(material_), x0(x_0), x1(x_1), y0(y_0), y1(y_1), k(k_)
    {

    }

    virtual bool hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const override;
    virtual bool boundingBox(double time0, double time1, aabb& output_box) const override;

private:
    double x0, x1, y0, y1, k;
    shared_ptr<Material> material;
};

bool Rect_xy::hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const
{
    auto t = (k - r.origin().z()) / r.direction().z();

    if (t < min_t || t > max_t)
        return false;

    auto x = r.origin().x() + t * r.direction().x();
    auto y = r.origin().y() + t * r.direction().y();

    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    hitrecord.u = (x - x0) / (x1 - x0);
    hitrecord.v = (y - y0) / (y1 - y0);
    hitrecord.t = t;
    auto outward_normal = Vec3(0, 0, 1);
    hitrecord.set_face_normal(r, outward_normal);
    hitrecord.material_ptr = material;
    hitrecord.p = r.at(t);
    return true;
}

bool Rect_xy::boundingBox(double time0, double time1, aabb& output_box) const
{
    output_box = aabb(Point3(x0, y0, k - 0.0001), Point3(x1, y1, k + 0.0001));
    return true;
}

class Rect_xz : public Hittable
{
public:
    Rect_xz()
    {
        x0 = 0;
        x1 = 0;
        z0 = 0;
        z1 = 0;
    }

    Rect_xz(shared_ptr<Material> material_, double x_0, double x_1, double z_0, double z_1, double k_)
        : material(material_), x0(x_0), x1(x_1), z0(z_0), z1(z_1), k(k_)
    {

    }

    virtual bool hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const override;
    virtual bool boundingBox(double time0, double time1, aabb& output_box) const override;

private:
    double x0, x1, z0, z1, k;
    shared_ptr<Material> material;
};

bool Rect_xz::hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const
{
    auto t = (k - r.origin().y()) / r.direction().y();

    if (t < min_t || t > max_t)
        return false;

    auto x = r.origin().x() + t * r.direction().x();
    auto z = r.origin().z() + t * r.direction().z();

    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;

    hitrecord.u = (x - x0) / (x1 - x0);
    hitrecord.v = (z - z0) / (z1 - z0);
    hitrecord.t = t;
    auto outward_normal = Vec3(0, 1, 0);
    hitrecord.set_face_normal(r, outward_normal);
    hitrecord.material_ptr = material;
    hitrecord.p = r.at(t);
    return true;
}

bool Rect_xz::boundingBox(double time0, double time1, aabb& output_box) const
{
    output_box = aabb(Point3(x0, k - 0.0001, z0), Point3(x1, k + 0.0001, z1));
    return true;
}

class Rect_yz : public Hittable
{
public:
    Rect_yz()
    {
        y0 = 0;
        y1 = 0;
        z0 = 0;
        z1 = 0;
    }

    Rect_yz(shared_ptr<Material> material_, double z_0, double z_1, double y_0, double y_1, double k_)
        : material(material_), z0(z_0), z1(z_1), y0(y_0), y1(y_1), k(k_)
    {

    }

    virtual bool hit(const Ray& r, const double& min_t, const double& maz_t, HitRecord& hitrecord) const override;
    virtual bool boundingBox(double time0, double time1, aabb& output_box) const override;

private:
    double z0, z1, y0, y1, k;
    shared_ptr<Material> material;
};

bool Rect_yz::hit(const Ray& r, const double& min_t, const double& max_t, HitRecord& hitrecord) const
{
    auto t = (k - r.origin().x()) / r.direction().x();

    if (t < min_t || t > max_t)
        return false;

    auto z = r.origin().z() + t * r.direction().z();
    auto y = r.origin().y() + t * r.direction().y();

    if (z < z0 || z > z1 || y < y0 || y > y1)
        return false;

    hitrecord.u = (z - z0) / (z1 - z0);
    hitrecord.v = (y - y0) / (y1 - y0);
    hitrecord.t = t;
    auto outward_normal = Vec3(1, 0, 0);
    hitrecord.set_face_normal(r, outward_normal);
    hitrecord.material_ptr = material;
    hitrecord.p = r.at(t);
    return true;
}

bool Rect_yz::boundingBox(double time0, double time1, aabb& output_box) const
{
    output_box = aabb(Point3(k - 0.0001, y0, z0), Point3(k + 0.0001, y1, z1));
    return true;
}