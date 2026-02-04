#ifndef MATERIALH
#define MATERIALH

#include "hitable.h"

class Material
{
    public:
        virtual ~Material() = default;
        virtual bool Scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const = 0;
};

// Lambertian (diffuse) material
class Lambertian : public Material
{
public:
    Lambertian(const vec3& a) : mAlbedo(a) {}
    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const override;
private:
    vec3 mAlbedo;
};

// Metal material
class Metal : public Material
{
public:
    Metal(const vec3& albedo) : mAlbedo(albedo), mFuzz(0.f) {}
    Metal(const vec3& albedo, float fuzz) : mAlbedo(albedo) { if (fuzz < 1) mFuzz = fuzz; else mFuzz = 1.; }
    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const override;
private:
    vec3 Reflect(const vec3& v_in, const vec3& normal) const;

    vec3 mAlbedo;
    float mFuzz;
};

// Dielectric (glass-like) material
class Dielectric : public Material
{
public:
    Dielectric(float ri) : mRefIdx(ri) {}
    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const override;
private:
    vec3 Reflect(const vec3& v_in, const vec3& normal) const;
    bool Refract(const vec3& v_in, const vec3& normal, float ni_over_nt, vec3& refracted) const;
    float Schlick(float cosine, float ref_idx) const;

    float mRefIdx;
};

#endif