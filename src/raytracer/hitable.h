#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class material; // Forward declaration

struct hit_record
{
    float t; // parameter t where ray hits
    vec3 p; // point of intersection
    vec3 normal; // normal at the intersection
    material *mat_ptr; // pointer to the material of the hit object
};

class hitable
{
    public:
        virtual ~hitable() = default;
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif