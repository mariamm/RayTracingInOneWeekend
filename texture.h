#pragma once

#include "util.h"
#include "vec3.h"

using std::shared_ptr;
using std::make_shared;

class Texture 
{
    public:
    virtual Color colorValue(double u, double v, const point3& p) const = 0;
};

class SolidColor : public Texture
{
    public:
        SolidColor() { color_value = Color(0, 0, 0); }

        SolidColor(Color c) : color_value(c){}
        SolidColor(double r, double g, double b)
        {
            color_value = Color(r, g, b);
        }

        virtual Color colorValue(double u, double v, const point3& p) const override
        {
            return color_value;
        }

    private:
        Color color_value;
};

class CheckeredTexture : public Texture
{
    public:
        CheckeredTexture()
        {
            even = make_shared<SolidColor>(Color(0,0,0));
            odd = make_shared<SolidColor>(Color(1, 1, 1));
        }

        CheckeredTexture(shared_ptr<Texture> even_, shared_ptr<Texture> odd_)
        {
            even = even_;
            odd = odd_;
        }

        CheckeredTexture(Color even_, Color odd_) 
        {
            even = make_shared<SolidColor>(even_);
            odd = make_shared<SolidColor>(odd_);
        }

        virtual Color colorValue(double u, double v, const point3& p) const override
        {
            auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
            if (sines < 0)
                return odd->colorValue(u, v, p);
            else
                return even->colorValue(u, v, p);
        }

    private:
        shared_ptr<Texture> even;
        shared_ptr<Texture> odd;
};

class ImageTexture : public Texture
{
    public:
        const static int bytesPerPixel = 3;
        ImageTexture() 
        {
            image = nullptr;

            width = 0;
            height = 0; 
            bytesPerScanLine = 0;
        }

        ImageTexture(const char* filename)
        {
            int componentsPerPixel = bytesPerPixel;
            image = stbi_load(filename, &width, &height, &componentsPerPixel, componentsPerPixel);

            if (!image)
            {
                std::cerr << "Couldn't load image";
            }
            bytesPerScanLine = componentsPerPixel * width;

        }
        ~ImageTexture()
        {
            stbi_image_free(image);
            image = nullptr;
        }

        virtual Color colorValue(double u, double v, const point3& p) const override
        {
            if (image == nullptr)
                return Color(0, 1, 1);

            // Clamp input texture coordinates to [0,1] x [1,0]
            u = clamp(u, 0.0, 1.0);
            v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

            auto i = static_cast<int>(u * width);
            auto j = static_cast<int>(v * height);

            // Clamp integer mapping, since actual coordinates should be less than 1.0
            if (i >= width)  i = width - 1;
            if (j >= height) j = height - 1;

            const auto color_scale = 1.0 / 255.0;
            auto pixel = image + j * bytesPerScanLine + i * bytesPerPixel;

            return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
        }

        private:
            unsigned char* image;
            int width, height;
            int bytesPerScanLine; //per row
};
