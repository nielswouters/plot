#ifndef CAMERAH
#define CAMERAH

#include "ray.h"
#include "random.h"

class camera
{
    public:
        // Default constructor with standard camera settings
        camera() : camera(vec3(0., 0., 0.), vec3(0., 0., -1.), vec3(0., 1., 0.), 90., 2.0, 0., 1.0) {}

        camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture = 0., float focus_dist = 1.0) // vfov is top to bottom in degrees
        {
            lens_radius = aperture / 2;
            float theta = vfov * M_PI / 180.;
            float half_height = tan(theta/2);
            float half_width = aspect * half_height;

            w = (lookfrom - lookat).unit_vector();
            u = cross(vup, w).unit_vector();
            v = cross(w, u);

            origin = lookfrom;
            left_upper_corner = origin - half_width * focus_dist * u + half_height * focus_dist * v - focus_dist * w;
            horizontal = 2 * half_width * focus_dist * u;
            vertical = 2 * half_height * focus_dist * v;
        }

        ray get_ray(float s, float t) const
        {
            if (lens_radius > 0) {
                vec3 rd = lens_radius * random_in_unit_disk();
                vec3 offset = u * rd.x() + v * rd.y();
                return ray(origin + offset, left_upper_corner + s*horizontal - t*vertical - origin - offset);
            }
            return ray(origin, left_upper_corner + s*horizontal - t*vertical - origin);
        };

        vec3 left_upper_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 origin;
        vec3 u, v, w;
        float lens_radius;
    private:
        vec3 random_in_unit_disk() const
        {
            vec3 p;
            do 
            {
                p = 2.0 * vec3(random_float(), random_float(), 0) - vec3(1, 1, 0);
            } while (dot(p, p) >= 1.0);
            return p;
        }
};

#endif