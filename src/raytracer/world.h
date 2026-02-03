#ifndef WORLDH
#define WORLDH

#include "sphere.h"
#include "hitablelist.h"

class world : public hitable_list
{
    public:
        world()
        {
            hitable *list[3];
            list[0] = new sphere(vec3(-0.5,0,-1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
            list[1] = new sphere(vec3(0,-100.5,-1), 100., new lambertian(vec3(0.8, 0.8, 0.0)));
            list[2] = new sphere(vec3(0.5,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
            hitable *world = new hitable_list(list, 3);
        }
        world(hitable **l, int n) : hitable_list(l, n) {}
};

#endif