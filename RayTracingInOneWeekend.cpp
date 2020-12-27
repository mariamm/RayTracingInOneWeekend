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
#include "bvh_node.h"
#include "axis_rectangle.h"
#include "box.h"

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
    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    return world;
}
HittableList random_scene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(COLOR_GREY);

    HittableList smallSpheres;
    for (int a = -11; a < 11; a++) 
    {
        for (int b = -11; b < 11; b++) 
        {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) 
            {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) 
                {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);

                    auto center2 = center + Vec3(0, random_double(0, .5), 0);
                    smallSpheres.add(make_shared<MovingSphere>(0.0, 1.0,
                        center, center2,  0.2, sphere_material));
                }
                else if (choose_mat < 0.95) 
                {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    smallSpheres.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else 
                {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    smallSpheres.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }


    HittableList largeSpheres;

    auto material1 = make_shared<Dielectric>(1.5);
    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);

    largeSpheres.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));
    largeSpheres.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));
    largeSpheres.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    world.add(make_shared<BvhNode>(smallSpheres, 0, 1));
    world.add(make_shared<BvhNode>(largeSpheres, 0, 1));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));
    return world;
}
HittableList earth_scene()
{
    shared_ptr<Texture> earthTexture = make_shared<ImageTexture>("textures\\earthmap.jpg");
    shared_ptr<Material> earthMaterial = make_shared<Lambertian>(earthTexture);
    
    HittableList world;
    world.add(make_shared<Sphere>(Point3(), 2, earthMaterial));
    return world;
}
HittableList simple_light() 
{
    HittableList world;

    auto checker = make_shared<CheckeredTexture>(Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9));
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, make_shared<Lambertian>(checker)));

    auto light_material = make_shared<Light>(COLOR_WHITE, 7);
    auto lambertian_material = make_shared<Lambertian>(COLOR_WHITE);

    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, lambertian_material));
    world.add(make_shared<Sphere>(Point3(0.0, 100.0, -1.0), 50, light_material));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, lambertian_material));


    world.add(make_shared<Box>(lambertian_material, Point3(-0.2, 0, -1.4), Point3(0.2, 0.4, -1)));
    
    return world;
}
HittableList cornell_box()
{
    HittableList world;

    auto red_material = make_shared<Lambertian>(Color(.65, .05, .05));
    auto green_material = make_shared<Lambertian>(Color(.12, .45, .15));
    auto white_material = make_shared<Lambertian>(Color(.73, .73, .73));
    auto light_material = make_shared<Light>(COLOR_WHITE, 15);

    //add walls
    world.add(make_shared<Rect_xz>(light_material, 213, 343, 227, 332, 554));
    world.add(make_shared<Rect_yz>(red_material,   0, 555, 0, 555, 555));
    world.add(make_shared<Rect_yz>(green_material, 0, 555, 0, 555, 0));
    world.add(make_shared<Rect_xz>(white_material, 0, 555, 0, 555, 0));
    world.add(make_shared<Rect_xz>(white_material, 0, 555, 0, 555, 555));
    world.add(make_shared<Rect_xy>(white_material, 0, 555, 0, 555, 555));

    //Add boxes
    world.add(make_shared<Box>(white_material, Point3(130, 0, 65), Point3(295, 165, 230)));
    world.add(make_shared<Box>(white_material, Point3(265, 0, 295), Point3(430, 330, 460)));

    return world;
}

Color calculate_color(Color pixel_color, int samples_per_pixel)
{
    double avg = 1. / samples_per_pixel;
    double r = clamp(pixel_color.x() * avg, 0., 0.999);
    double g = clamp(pixel_color.y() * avg, 0., 0.999);
    double b = clamp(pixel_color.z() * avg, 0., 0.999);

    //gamma corrected:
    r = std::sqrt(r);
    g = std::sqrt(g);
    b = std::sqrt(b);

    return Color((255.999 * r),(255.999 * g), (255.999 * b));
}

void write_color(std::ofstream& out, Color pixel_color, int samples_per_pixel)
{
    Color c = calculate_color(pixel_color, samples_per_pixel);
    out << static_cast<int>(255.999 * c.x()) << ' '
        << static_cast<int>(255.999 * c.y()) << ' '
        << static_cast<int>(255.999 * c.z()) << '\n';
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

Color get_ray_color(const Ray& r, const Hittable &world, int depth, const Color &backgroundColor)
{
    HitRecord rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Color();


    //Setting t_min = 0.001 instead of 0 gets rid of the shadow acne problem
    if (!world.hit(r, 0.001, infinity, rec))
    {
        //nothing hit, return background color
        return backgroundColor;
    }

    Ray scattered;
    Color attenuation; //value of obsorbed color 
    Color emitted = rec.material_ptr->color_emitted(rec.u, rec.v, rec.p); //if Material is light emitting
    if (rec.material_ptr->scatter(r, rec, attenuation, scattered))
    {
        return emitted + attenuation * get_ray_color(scattered, world, depth - 1, backgroundColor);
    }
    else
        return emitted;
}

void writeImage(const unsigned char* image, const char* filename, int h, int w, int color_channels) 
{
    int result = stbi_write_png(filename, w, h, color_channels, image, 3 * w);

}
int main()
{
    // World
    HittableList world;
    Color background(0, 0, 0);

    // Camera
    Point3 cameraPosition(0, 0, 7);
    Point3 cameraLookAt(0, 0, 0);
    Vec3 cameraUp(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    double fieldOfView_deg = 20.;

    int choice = 5;
    std::cout << "Choose scene: \n"
        "1 - Random Scene (Book 1 cover) \n"
        "2 - Initial Scene (3 Spheres) \n"
        "3 - Simple light scene \n"
        "4 - Earth Scene (with JPG Texture)\n" 
        "5 - Cornell Box"<< std::endl;


    std::cin >> choice;

    //Image
    const char* imagepng = "result_images\\Cornellbox.png";
    const auto aspect_ratio = 1.0;// 3.0 / 2.0;
    const int w = 600;
    const int h = static_cast<int>(w / aspect_ratio);
    size_t size = w * h * 3;
    unsigned char* image = new unsigned char[size];

    //Rendering Parameters
    const int samples_per_pixel = 200;
    const int max_depth = 30;

    switch (choice) 
    {
    case 1:
        world = random_scene();
        background = Color(0.70, 0.80, 1.00);
        cameraPosition = Point3(13, 2, 3);
        cameraLookAt = Point3(0, 0, 0);
        fieldOfView_deg = 20.0;
        aperture = 0.1;
        break;

    case 2:
        world = initial_scene();
        background = Color(0.70, 0.80, 1.00);
        cameraPosition = Point3(0, 0, 7);
        cameraLookAt = Point3(0, 0, 0);
        fieldOfView_deg = 20.0;
        aperture = 0.1;
        break;

    case 3:
        world = simple_light();
        background = Color(0.0, 0.0, 0.0);    
        cameraLookAt = Point3(0, 0, 0);
        fieldOfView_deg = 20.0;
        aperture = 0.1;
        break;

    case 4:
        world = earth_scene();
        background = Color(0.70, 0.80, 1.00);
        cameraPosition = Point3(13, 2, 3);
        cameraLookAt = Point3(0, 0, 0);
        fieldOfView_deg = 20.0;
        break;
    case 5:
        world = cornell_box();
        background = Color(0.1, 0.1, 0.1); //to add some light 
        cameraPosition = Point3(278, 278, -800);
        cameraLookAt = Point3(278, 278, 0);
        fieldOfView_deg = 40.0;
        break;

    default:
        background = Color(0.0, 0.0, 0.0);
        break;
    }


    Camera cam(cameraPosition, cameraLookAt, cameraUp, fieldOfView_deg, aspect_ratio, dist_to_focus, aperture);

    //render

    for (int j = h - 1; j >= 0; j--)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        #pragma omp parallel for
        for (int i = 0; i < w; i++)
        {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) 
            {
                auto u = double(i + random_double()) / (w - 1);
                auto v = double(j + random_double()) / (h - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += get_ray_color(r, world, max_depth, background);
            }

            Color c = calculate_color(pixel_color, samples_per_pixel);
            
            int invertJ = h - j - 1;
            int x = 3 * invertJ * w + 3*i;
            image[x] = static_cast<unsigned char>(c.x());
            image[x+1] = static_cast<unsigned char>(c.y());
            image[x+2] = static_cast<unsigned char>(c.z());
        }
    }

    writeImage(image, imagepng, h, w, 3);

    delete[] image;

    std::cerr << "\nDone.\n";

}