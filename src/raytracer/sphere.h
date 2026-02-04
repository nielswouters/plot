#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"
#include "material.h"

class Sphere : public Hitable
{
    public:
        Sphere() {}
        Sphere(vec3 cen, float r) : mCenter(cen), mRadius(r) {}
        Sphere(vec3 cen, float r, Material *m) : mCenter(cen), mRadius(r), mMatPtr(m) {}
        virtual bool Hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const override;

        vec3  mCenter;
        float mRadius;
        Material *mMatPtr;
};

#endif