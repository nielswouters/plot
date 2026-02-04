#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class Camera
{
public:
    // Default constructor with standard camera settings
    Camera();

    // vfov is top to bottom in degrees;
    Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture = 0., float focus_dist = 1.0);

    Ray get_ray(float s, float t) const;

    vec3 GetOrigin() const { return origin; }

private:
    vec3 random_in_unit_disk() const;

    vec3 left_upper_corner;
    vec3 mHorizontal;
    vec3 vertical;
    vec3 origin;
    vec3 u, v, w;
    float lens_radius;
};

#endif
