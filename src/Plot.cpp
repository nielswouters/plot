#include "Plot.h"

#include "Surface.h"
#include "raytracer/camera.h"
#include "raytracer/hitable.h"
#include "raytracer/hitablelist.h"
#include "raytracer/sphere.h"

#include <cfloat>
#include <stdio.h>

Plot::Plot()
    : mFPS(0), world(nullptr), initialized(false)
{
    materials[0] = nullptr;
    materials[1] = nullptr;
    materials[2] = nullptr;
    list[0] = nullptr;
    list[1] = nullptr;
    list[2] = nullptr;
}

Plot::~Plot()
{
    delete world;
    delete list[0];
    delete list[1];
    delete list[2];
    delete materials[0];
    delete materials[1];
    delete materials[2];
}

void Plot::MouseMove(uint32_t _X, uint32_t _Y)
{
    printf("Mouse Move: %u %u\n", _X, _Y);
}

void Plot::MouseButton(uint32_t _Button, bool _Down)
{
    printf("Mouse Button: %u %s\n", _Button, _Down ? "Down" : "Up");
}

void Plot::KeyboardButton(uint32_t _Button, bool _Down)
{
    printf("Keyboard Button: %u %s\n", _Button, _Down ? "Down" : "Up");
}

void Plot::Update(float _DeltaTime)
{
    mFPS = (uint32_t)(1000.0f / _DeltaTime);
}

static vec3 color(const Ray& r, Hitable *world, int depth = 0)
{
    HitRecord rec;
    if (world->Hit(r, 0.001, FLT_MAX, rec))
    {
        Ray scattered;
        vec3 attenuation;
        if (depth < 5 && rec.mMatPtr->Scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0., 0., 0.);
        }
    }
    else
    {
        vec3 unit_direction = r.Direction().unit_vector();
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0-t) * vec3(1., 1., 1.) + t * vec3(0.5, 0.7, 1.0);
    }
}

void Plot::Draw(Surface* _Screen)
{
    _Screen->Clear();

    static uint32_t offset = 0;
    offset++;

    static Camera cam;

    if (!initialized) {
        materials[0] = new Lambertian(vec3(0.8, 0.3, 0.3));
        materials[1] = new Lambertian(vec3(0.8, 0.8, 0.0));
        // materials[2] = new lambertian(vec3(0.8, 0.6, 0.2));
        materials[2] = new Metal(vec3(0.8, 0.6, 0.2), 0.8);

        list[0] = new Sphere(vec3(0,0,-1), 0.5, materials[0]);
        list[1] = new Sphere(vec3(0,-100.5,-1), 100., materials[1]);
        list[2] = new Sphere(vec3(1,0,-1), 0.5, materials[2]);

        world = new hitable_list(list, 3);
        initialized = true;
    }

    uint32_t width = _Screen->GetWidth();
    uint32_t height = _Screen->GetHeight();

    
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {   
            vec3 col(0., 0., 0.);

            Ray first_ray = cam.get_ray(float(x) / float(width), float(y) / float(height));
            col = color(first_ray, world, 0);
            col = sqrt(col);
            col *= 255.99;
            uint32_t r = uint32_t(col.r());
            uint32_t g = uint32_t(col.g());
            uint32_t b = uint32_t(col.b());

            uint32_t c = r << 16 | g << 8 | b;   

            _Screen->Plot(x, y, c);
        }
    }

    char text[64];
    snprintf(text, sizeof(text), "FPS: %u", mFPS);
    _Screen->Print(10, 10, text, 0xFFFFFF);
}
