#ifndef RAYH
#define RAYH

#include "vec3.h"

class ray
{
    public:
        // Constructors
        ray() {}
        ray(const vec3& a, const vec3& b) { A = a; B = b; }

        // Member functions
        vec3 origin() const { return A; }
        vec3 direction() const { return B; }
        vec3 point_at_parameter(float t) const { return A + t * B; }

        // Member variables  
        vec3 A;
        vec3 B;
};

#endif