#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"
#include "material.h"

class sphere : public hitable
{
    public:
        sphere() {}
        sphere(vec3 cen, float r) : center(cen), radius(r) {}
        sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {}
        virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const override;

        vec3  center;
        float radius;
        material *mat_ptr;
};

inline bool sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
{
    vec3 oc = r.GetOrigin() - center;
    float a = r.Direction().length_squared();
    float b  = dot(oc, r.Direction());
    float c  = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0)
    {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t      = temp;
            rec.p      = r.At(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }

        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t      = temp;
            rec.p      = r.At(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;  
            return true;
        }
    }
    return false;
}

#endif