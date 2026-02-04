#include "hitablelist.h"

bool hitable_list::Hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (int i = 0; i < mListSize; i++)
    {
        if (mList[i]->Hit(ray, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.mParameterOfHit;
            rec = temp_rec;
        }
    }
    return hit_anything;
}