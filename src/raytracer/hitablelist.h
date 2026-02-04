#ifndef HITABLELIST
#define HITABLELIST

#include "hitable.h"

class hitable_list: public Hitable
{
public:
    hitable_list() {}
    hitable_list(Hitable **list, int listsize) : mList(list), mListSize(listsize) {}
private:
    virtual bool Hit(const Ray& ray, float tmin, float tmax, HitRecord& rec) const;

    Hitable **mList;
    int mListSize;
};

#endif