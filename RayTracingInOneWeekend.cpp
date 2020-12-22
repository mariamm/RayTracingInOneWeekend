// RayTracingInOneWeekend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

#include "util.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"


using std::shared_ptr;
using std::make_shared;

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) 
    {
        for (int b = -11; b < 11; b++) 
        {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) 
            {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) 
                {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Lambertian>(albedo);

                    auto center2 = center + vec3(0, random_double(0, .5), 0);
                    world.add(make_shared<moving_sphere>(0.0, 1.0,
                        center, center2,  0.2, sphere_material));
                }
                else if (choose_mat < 0.95) 
                {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else 
                {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

void write_color(std::ofstream& out, color pixel_color, int samples_per_pixel)
{

    double avg = 1. / samples_per_pixel;
    double r = clamp(pixel_color.x() * avg, 0., 0.999);
    double g = clamp(pixel_color.y() * avg, 0., 0.999);
    double b = clamp(pixel_color.z() * avg, 0., 0.999);

    //gamma corrected:
    r = std::sqrt(r);
    g = std::sqrt(g);
    b = std::sqrt(b);
    out << static_cast<int>(255.999 * r) << ' '
        << static_cast<int>(255.999 * g) << ' '
        << static_cast<int>(255.999 * b) << '\n';
}

bool createGradientPPM(std::string filename)
{
    std::ofstream myfile;
    myfile.open(filename + ".ppm");
    if (!myfile.is_open())
    {
        return false;
    }
    //image
    const int image_width = 256;
    const int image_height = 256;

    //render
    myfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; j--)
    {
        for (int i=0; i<image_width; i++)
        {
            double r = double(i) / (image_width-1);
            double g = double(j) / (image_height-1);
            double b = 0.25;

            color pixelcolor(r, g, b);
            write_color(myfile, pixelcolor, 100);
        }
    }

    myfile.close();
    return true;
} 

color ray_color(const ray& r, const hittable &world, int depth)
{
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);
    //Setting t_min = 0.001 instead of 0 gets rid of the shadow acne problem
    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        color attenuation; //value of obsorbed color 
        if (rec.material_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return color(0, 0, 0);
    }
    //return background
    vec3 unit_dir = unit_vector(r.direction());
    double t = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


int main()
{
    // World
    auto world = random_scene();

    //Image
    std::string imagename = "image13Final.ppm";
    const auto aspect_ratio = 3.0 / 2.0;
    const int w = 400;
    const int h = static_cast<int>(w / aspect_ratio);
    const int samples_per_pixel = 40;
    const int max_depth = 10;

    // Camera
    point3 cameraPosition(13, 2, 3);
    point3 cameraLookAt(0, 0, 0);
    vec3 cameraUp(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    double fieldOfView_deg = 20.;

    Camera cam(cameraPosition, cameraLookAt, cameraUp, fieldOfView_deg, aspect_ratio, dist_to_focus, aperture);

    std::ofstream myfile;
    myfile.open(imagename);
    if (!myfile.is_open())
    {
        return 0;
    }

    //render
    myfile << "P3\n" << w << ' ' << h << "\n255\n";
    for (int j = h - 1; j >= 0; j--)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < w; i++)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = double(i) / (w - 1);
                auto v = double(j) / (h - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(myfile, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
    myfile.close();


}