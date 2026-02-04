#include "material.h"

#include "random.h"


inline vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0 * vec3(random_float(), random_float(), random_float()) - vec3(1., 1., 1.);
    } while (p.length_squared() >= 1.);
    return p;    
}

// Lambertian (diffuse) material
bool Lambertian::Scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const
{            
    vec3 target = rec.mPointOfHit + rec.mNormal + random_in_unit_sphere();
    scattered = Ray(rec.mPointOfHit, target - rec.mPointOfHit);
    attenuation = mAlbedo;
    return true;
}

// Metal material
bool Metal::Scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const
    {
        vec3 reflected = Reflect(r_in.Direction().unit_vector(), rec.mNormal);
        scattered = Ray(rec.mPointOfHit, reflected + mFuzz*random_in_unit_sphere());
        attenuation = mAlbedo;
        return (dot(scattered.Direction(), rec.mNormal) > 0);
    }

vec3 Metal::Reflect(const vec3& v_in, const vec3& normal) const
{
    return v_in - 2 * dot(v_in, normal) * normal;
};

// Dielectric (glass-like) material
bool Dielectric::Scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const
{
    vec3 outward_normal;
    vec3 reflected = Reflect(r_in.Direction(), rec.mNormal);
    attenuation = vec3(1.0, 1.0, 1.0); // no attenuation for dielectric
    vec3 refracted;
    float reflect_prob;
    float cosine;
    float ni_over_nt;

    if (dot(r_in.Direction(), rec.mNormal) > 0)
    {
        outward_normal = -rec.mNormal;
        ni_over_nt = mRefIdx;
        cosine = mRefIdx * dot(r_in.Direction(), rec.mNormal) / r_in.Direction().length();
    }
    else
    {
        outward_normal = rec.mNormal;
        ni_over_nt = 1.0 / mRefIdx;
        cosine = -dot(r_in.Direction(), rec.mNormal) / r_in.Direction().length();
    }

    if (Refract(r_in.Direction(), outward_normal, ni_over_nt, refracted))
    {
        reflect_prob = Schlick(cosine, mRefIdx);
    }
    else
    {
        reflect_prob = 1.0;
    }

    if (random_float() < reflect_prob)
    {
        scattered = Ray(rec.mPointOfHit, reflected);
    }
    else
    {
        scattered = Ray(rec.mPointOfHit, refracted);
    }

    return true;
}

vec3 Dielectric::Reflect(const vec3& v_in, const vec3& normal) const
{
    return v_in - 2 * dot(v_in, normal) * normal;
};

bool Dielectric::Refract(const vec3& v_in, const vec3& normal, float ni_over_nt, vec3& refracted) const
{
    vec3 uv = v_in.unit_vector();
    float dt = dot(uv, normal);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - normal * dt) - normal * sqrt(discriminant);
        return true;
    }
    else
    {
        return false;
    }
};

float Dielectric::Schlick(float cosine, float ref_idx) const
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
};