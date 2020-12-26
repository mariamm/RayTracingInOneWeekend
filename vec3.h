#pragma once

#include "util.h"
#include <cmath>
#include <iostream>

class Vec3 {

public:
    Vec3() :v{ 0.,0.,0. } {}

    Vec3(double x, double y, double z) :v{ x, y, z } {}

    //get x, y, z values
    double x() const { return v[0]; };
    double y() const { return v[1]; };
    double z() const { return v[2]; };

    double operator[](int i) const { assert(i >= 0 && i < 3);  return v[i]; }
    double& operator[](int i) { assert(i >= 0 && i < 3); return v[i]; }

    //get negative (opposite direction) vector
    Vec3 operator-() const {
        return { -x(), -y(), -z() };
    }

    //add to vector
    Vec3& operator+=(const Vec3& other)
    {
        v[0] += other.x();
        v[1] += other.y();
        v[2] += other.z();

        return *this;
    }

    //scalar product
    Vec3& operator*=(const double scalar)
    {
        v[0] *= scalar;
        v[1] *= scalar;
        v[2] *= scalar;
        return *this;
    }

    Vec3& operator/=(const double scalar)
    {
        return *this *= 1. / scalar;
    }

    double length_squared() const {
        return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    }

    double length() const
    {
        return std::sqrt(length_squared());
    }

    inline static Vec3 random(double minPoint, double maxPoint) 
    {
        return Vec3(random_double(minPoint, maxPoint), random_double(minPoint, maxPoint), random_double(minPoint, maxPoint));
    }

    inline static Vec3 random() {
        return Vec3(random_double(), random_double(), random_double());
    }
    
    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(v[0]) < s) && (fabs(v[1]) < s) && (fabs(v[2]) < s);
    }
private:
    double v[3];

};
//Type alisases for vec3
using Point3 = Vec3;
using Color = Vec3; // normalized color values rgb [0,1]

#define     COLOR_WHITE   Color(1, 1, 1)
#define     COLOR_BLACK   Color(0, 0, 0)
#define     COLOR_GREY    Color(0.5, 0.5, 0.5)    
#define     COLOR_RED     Color(1, 0, 0)
#define     COLOR_GREEN   Color(0, 1, 0)
#define     COLOR_BLUE    Color(0, 0, 1)
#define     COLOR_CYAN    Color(0, 1, 1)
#define     COLOR_TEAL    Color(0, 0.5, 0.5)
#define     COLOR_YELLOW  Color(1, 1, 0)
#define     COLOR_GOLD    Color(1, 0.84, 0) 
#define     COLOR_OLIVE   Color(0.5, 0.5, 0)
#define     COLOR_MAGENTA Color(1, 0, 1)
#define     COLOR_PURPLE  Color(0.5, 0, 0.5)

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    return out << v.x() << v.y() << v.z();
}

inline Vec3 operator+(const Vec3& v, double offset)
{
    return Vec3(v.x() + offset, v.y() + offset, v.z() + offset);
}

inline Vec3 operator-(const Vec3& v, double offset)
{
    return Vec3(v.x() - offset, v.y() - offset, v.z() - offset);
}

inline Vec3 operator+(const Vec3& v, const Vec3& u)
{
    return Vec3(v.x() + u.x(), v.y() + u.y(), v.z() + u.z());
}

inline Vec3 operator-(const Vec3& v, const Vec3& u)
{
    return Vec3(v.x() - u.x(), v.y() - u.y(), v.z() - u.z());
}

inline Vec3 operator*(const Vec3& v, const Vec3& u)
{
    return Vec3(v.x() * u.x(), v.y() * u.y(), v.z() * u.z());
}
inline Vec3 operator*(const Vec3& v, const double &t)
{
    return Vec3(v.x()*t, v.y()*t, v.z()*t);
}
inline Vec3 operator*(const double& t, const Vec3& v)
{
    return v * t;
}

inline Vec3 operator/(Vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vec3& v, const Vec3& u)
{
    return (v.x() * u.x()+ v.y()* u.y()+ v.z()* u.z());
}

inline Vec3 cross(const Vec3& v, const Vec3& u)
{
    return Vec3(
        v.y() * u.z() - v.z() * u.y(),
        v.z() * u.x() - v.x() * u.z(),
        v.x() * u.y()- v.y() * u.x()
    );
}

inline Vec3 unit_vector( Vec3 v)
{
    return v / v.length();
}

Vec3 random_in_unit_disk() 
{
    while (true) 
    {
        auto p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}
Vec3 random_in_unit_sphere()
{
    /*pick a random point in a unit radius sphere. We’ll use what is usually the easiest algorithm:
    a rejection method. First, pick a random point in the unit cube where x, y, and z all range from −1 to +1.
    Reject this point and try again if the point is outside the sphere.*/
    while (true) 
    {
        auto p = Vec3::random(-1., 1.);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

Vec3 random_unit_vector() 
{
    return unit_vector(random_in_unit_sphere());
}

Vec3 random_in_hemisphere(const Vec3& normal) 
{
    Vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Vec3 reflect(const Vec3& v, const Vec3& n) 
{
    return v - 2 * dot(v, n) * n;
}

Vec3 refract(const Vec3& v, const Vec3& n, double eta_over_etaPrime) 
{
    //Formula R_perpendicular = eta/etaPrime ( R + (-R dot n) n)
    //Formula R_parallel = - sqrt(1 - |R_perpendicular|^2) * n
    //Formula R = R_perpendicular + R_parallel
    auto cos_theta = std::fmin(dot(-v, n), 1.0);
    Vec3 r_out_perp = eta_over_etaPrime * (v + cos_theta * n);
    Vec3 r_out_parallel = -std::sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}