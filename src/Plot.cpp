#include "Plot.h"

#include "Surface.h"
#include "raytracer/camera.h"
#include "raytracer/hitable.h"
#include "raytracer/hitablelist.h"
#include "raytracer/sphere.h"
// #include "raytracer/world.h"

#include <stdio.h>

Plot::Plot()
    : mFPS(0)
{

}

Plot::~Plot()
{

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

static vec3 color(const ray& r, hitable *world, int depth = 0)
{
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec))
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 5 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
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
        vec3 unit_direction = r.direction().unit_vector();
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0-t) * vec3(1., 1., 1.) + t * vec3(0.5, 0.7, 1.0);
    }
}

void Plot::Draw(Surface* _Screen)
{
    _Screen->Clear();

    static uint32_t offset = 0;
    offset++;

    static camera cam;
    // static world wld;

    static hitable *list[3] = {
        new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3))),
        new sphere(vec3(0,-100.5,-1), 100., new lambertian(vec3(0.8, 0.8, 0.0))),
        new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.8))
    };
    static hitable *world = new hitable_list(list, 3);

    uint32_t width = _Screen->GetWidth();
    uint32_t height = _Screen->GetHeight();

    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {   
            vec3 col(0., 0., 0.);

            ray first_ray = cam.get_ray(float(x) / float(width), float(y) / float(height));
            col = color(first_ray, world);
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
