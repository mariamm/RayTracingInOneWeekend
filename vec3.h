#pragma once

#include "util.h"
#include <cmath>
#include <iostream>

class vec3 {

public:
    vec3() :v{ 0.,0.,0. } {}

    vec3(double x, double y, double z) :v{ x, y, z } {}

    //get x, y, z values
    double x() const { return v[0]; };
    double y() const { return v[1]; };
    double z() const { return v[2]; };

    //get negative (opposite direction) vector
    vec3 operator-() const {
        return { -x(), -y(), -z() };
    }

    //add to vector
    vec3& operator+=(const vec3& other)
    {
        v[0] += other.x();
        v[1] += other.y();
        v[2] += other.z();

        return *this;
    }

    //scalar product
    vec3& operator*=(const double scalar)
    {
        v[0] *= scalar;
        v[1] *= scalar;
        v[2] *= scalar;
        return *this;
    }

    vec3& operator/=(const double scalar)
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

    inline static vec3 random(double min, double max) 
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
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
using point3 = vec3; 
using Color = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
    return out << v.x() << v.y() << v.z();
}

inline vec3 operator+(const vec3& v, const vec3& u)
{
    return vec3(v.x() + u.x(), v.y() + u.y(), v.z() + u.z());
}

inline vec3 operator-(const vec3& v, const vec3& u)
{
    return vec3(v.x() - u.x(), v.y() - u.y(), v.z() - u.z());
}

inline vec3 operator*(const vec3& v, const vec3& u)
{
    return vec3(v.x() * u.x(), v.y() * u.y(), v.z() * u.z());
}
inline vec3 operator*(const vec3& v, const double &t)
{
    return vec3(v.x()*t, v.y()*t, v.z()*t);
}
inline vec3 operator*(const double& t, const vec3& v)
{
    return v * t;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3& v, const vec3& u)
{
    return (v.x() * u.x()+ v.y()* u.y()+ v.z()* u.z());
}

inline vec3 cross(const vec3& v, const vec3& u)
{
    return vec3(
        v.y() * u.z() - v.z() * u.y(),
        v.z() * u.x() - v.x() * u.z(),
        v.x() * u.y()- v.y() * u.x()
    );
}

inline vec3 unit_vector( vec3 v)
{
    return v / v.length();
}

vec3 random_in_unit_disk() 
{
    while (true) 
    {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}
vec3 random_in_unit_sphere()
{
    /*pick a random point in a unit radius sphere. We’ll use what is usually the easiest algorithm:
    a rejection method. First, pick a random point in the unit cube where x, y, and z all range from −1 to +1.
    Reject this point and try again if the point is outside the sphere.*/
    while (true) 
    {
        auto p = vec3::random(-1., 1.);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

vec3 random_unit_vector() 
{
    return unit_vector(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3& normal) 
{
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

vec3 reflect(const vec3& v, const vec3& n) 
{
    return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3& v, const vec3& n, double eta_over_etaPrime) 
{
    //Formula R_perpendicular = eta/etaPrime ( R + (-R dot n) n)
    //Formula R_parallel = - sqrt(1 - |R_perpendicular|^2) * n
    //Formula R = R_perpendicular + R_parallel
    auto cos_theta = std::fmin(dot(-v, n), 1.0);
    vec3 r_out_perp = eta_over_etaPrime * (v + cos_theta * n);
    vec3 r_out_parallel = -std::sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
