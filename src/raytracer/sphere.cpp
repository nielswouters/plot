#include "sphere.h"

bool Sphere::Hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const
{
    vec3 oc = ray.GetOrigin() - mCenter;
    float a = ray.Direction().length_squared();
    float b  = dot(oc, ray.Direction());
    float c  = dot(oc, oc) - mRadius * mRadius;
    float discriminant = b * b - a * c;

    if (discriminant > 0)
    {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < tmax && temp > tmin)
        {
            rec.mParameterOfHit      = temp;
            rec.mPointOfHit      = ray.At(rec.mParameterOfHit);
            rec.mNormal = (rec.mPointOfHit - mCenter) / mRadius;
            rec.mMatPtr = mMatPtr;
            return true;
        }

        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < tmax && temp > tmin) {
            rec.mParameterOfHit      = temp;
            rec.mPointOfHit      = ray.At(rec.mParameterOfHit);
            rec.mNormal = (rec.mPointOfHit - mCenter) / mRadius;
            rec.mMatPtr = mMatPtr;  
            return true;
        }
    }
    return false;
}