#ifndef RAYH
#define RAYH

#include "vec3.h"

class Ray
{
public:
    Ray() {}
    Ray(const vec3& origin, const vec3& direction) : mOrigin(origin), mDirection(direction) {}

    vec3 GetOrigin() const { return mOrigin; }
    vec3 Direction() const { return mDirection; }
    vec3 At(float t) const { return mOrigin + t * mDirection; }

private:
    vec3 mOrigin;
    vec3 mDirection;
};

#endif
