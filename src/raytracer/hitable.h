#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class Material; // Forward declaration

struct HitRecord
{
    float mParameterOfHit; // parameter t where ray hits
    vec3 mPointOfHit; // point of intersection
    vec3 mNormal; // normal at the intersection
    Material *mMatPtr; // pointer to the material of the hit object
};

class Hitable
{
public:
    virtual ~Hitable() = default;
    virtual bool Hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const = 0;
};

#endif