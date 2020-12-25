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

HittableList initial_scene()
{
    HittableList world;

    auto checker = make_shared<CheckeredTexture>(Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9));
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    //world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    return world;
}
HittableList random_scene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
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
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);

                    auto center2 = center + vec3(0, random_double(0, .5), 0);
                    world.add(make_shared<moving_sphere>(0.0, 1.0,
                        center, center2,  0.2, sphere_material));
                }
                else if (choose_mat < 0.95) 
                {
                    // metal
                    auto albedo = Color::random(0.5, 1);
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

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

HittableList earth_scene()
{
    shared_ptr<Texture> earthTexture = make_shared<ImageTexture>("textures\\earthmap.jpg");
    shared_ptr<Material> earthMaterial = make_shared<Lambertian>(earthTexture);
    
    HittableList world;
    world.add(make_shared<Sphere>(point3(), 2, earthMaterial));
    return world;
}

HittableList simple_light() 
{
    HittableList world;

    auto checker = make_shared<CheckeredTexture>(Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9));
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto difflight = make_shared<DiffuseLight>(Color(4, 4, 4));
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, make_shared<Lambertian>(checker)));

    world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, difflight));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    return world;
}
void write_color(std::ofstream& out, Color pixel_color, int samples_per_pixel)
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
    myfile.open(filename);
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

            Color pixelcolor(r, g, b);
            write_color(myfile, pixelcolor, 100);
        }
    }

    myfile.close();
    return true;
} 

Color get_ray_color(const ray& r, const hittable &world, int depth, const Color &backgroundColor)
{
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Color();


    //Setting t_min = 0.001 instead of 0 gets rid of the shadow acne problem
    if (!world.hit(r, 0.001, infinity, rec))
    {
        //nothing hit, return background color
        return backgroundColor;
    }

    ray scattered;
    Color attenuation; //value of obsorbed color 
    Color emitted = rec.material_ptr->color_emitted(rec.u, rec.v, rec.p); //if Material is light emitting
    if (rec.material_ptr->scatter(r, rec, attenuation, scattered))
    {
        return emitted + attenuation * get_ray_color(scattered, world, depth - 1, backgroundColor);
    }
    else
        return emitted;
}


int main()
{
    // World
    //auto world = random_scene();
    HittableList world = initial_scene();
    //HittableList world = earth_scene();
    //HittableList world = simple_light();

    //Image
    std::string imagename = "image19.ppm";
    const auto aspect_ratio = 3.0 / 2.0;
    const int w = 400;
    const int h = static_cast<int>(w / aspect_ratio);
    const int samples_per_pixel = 70;
    const int max_depth = 15;
    Color background(0.7, 0.8, 1.0);

    // Camera
    //point3 cameraPosition(13, 2, 3);
    point3 cameraPosition(0, 0, 0);
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
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = double(i) / (w - 1);
                auto v = double(j) / (h - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += get_ray_color(r, world, max_depth, background);
            }

            write_color(myfile, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
    myfile.close();


}