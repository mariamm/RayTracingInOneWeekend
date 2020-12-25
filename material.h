#pragma once

#include "util.h"
#include "texture.h"
#include "ray.h"
#include "hittable.h"

class Material {
    public:
        virtual Color color_emitted(double u, double v, const point3& p) const 
        {
            return Color(0, 0, 0);
        }
        virtual bool scatter(const ray& ray_in, const hit_record& rec, Color& attenuation, ray& scatter_ray) const = 0;
};

class Lambertian : public Material
{
    public:
    Lambertian(const Color& a) : albedo(make_shared<SolidColor>(a)) {}
    Lambertian(shared_ptr<Texture> _albedo) : albedo(_albedo) {}
    
        virtual bool scatter(const ray& ray_in, const hit_record& rec, Color& attenuation, ray& scatter_ray) const override
        {
            //Diffuse reflection of randomly scattered rays
            auto scatter_direction = rec.normal + random_unit_vector();
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;
            scatter_ray = ray(rec.p, scatter_direction, ray_in.time());
            attenuation = albedo->colorValue(rec.u, rec.v, rec.p);
            return true;
        }
    private:
        shared_ptr<Texture> albedo;
};

class Metal : public Material
{
    public:
    Metal(Color _albedo, double _fuzz) : albedo(_albedo), fuzz(std::min(_fuzz, 1.0)) {}
    virtual bool scatter(const ray& ray_in, const hit_record& rec, Color& attenuation, ray& scatter_ray) const override
    {
        vec3 reflected = reflect(ray_in.direction(), rec.normal);
        //scattered is the left over of what wasn't directly reflected
        scatter_ray = ray(rec.p, reflected + fuzz* random_in_unit_sphere(), ray_in.time());
        attenuation = albedo;
        return (dot(scatter_ray.direction(), rec.normal) > 0);
    }
private:
    Color albedo;
    double fuzz;
};

class Dielectric : public Material
{
    public: 
        Dielectric( double refraction_index):  refractionIndex(refraction_index) {}
        virtual bool scatter(const ray& ray_in, const hit_record& rec, Color& attenuation, ray& scatter_ray) const override
        {
            attenuation = Color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0 / refractionIndex) : refractionIndex;

            vec3 unit_direction = unit_vector(ray_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;
            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scatter_ray = ray(rec.p, direction, ray_in.time());
            return true;
        }
    private:
        double refractionIndex; //air 1.0, glass 1.3-1.7, diamond 2.4

        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
};


class DiffuseLight : public Material
{
    public:

        DiffuseLight(shared_ptr<Texture> texture) : emit(texture){}
        DiffuseLight(Color color)
        {
            emit = make_shared<SolidColor>(color);
        }
        virtual bool scatter(const ray& ray_in, const hit_record& rec, Color& attenuation, ray& scatter_ray) const override
        {
            return false;
        }

        virtual Color color_emitted(double u, double v, const point3& p)
        {
            return emit->colorValue(u, v, p);
        }
        shared_ptr<Texture> emit;
};