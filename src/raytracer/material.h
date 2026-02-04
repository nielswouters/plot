#ifndef MATERIALH
#define MATERIALH

#include "ray.h"
#include "hitable.h"
#include "random.h"

class material
{
    public:
        virtual ~material() = default;
        virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const = 0;
};

// Diffuse/lambertian material
inline vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0 * vec3(random_float(), random_float(), random_float()) - vec3(1., 1., 1.);
    } while (p.length_squared() >= 1.);
    return p;    
}

class lambertian : public material
{
    public:
        lambertian(const vec3& a) : albedo(a) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const override
        {            
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = Ray(rec.p, target - rec.p);
            attenuation = albedo;
            return true;
        }

        vec3 albedo;
};

// Metal material
class metal : public material
{
    public:
        metal(const vec3& a) : albedo(a), fuzz(0.f) {}
        metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1.; }

        virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const override
        {
            vec3 reflected = reflect(r_in.Direction().unit_vector(), rec.normal);
            scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.Direction(), rec.normal) > 0);
        }
        vec3 albedo;
        float fuzz;
    private:
        vec3 reflect(const vec3& v, const vec3& n) const
        {
            return v - 2 * dot(v, n) * n;
        }
};

// Dielectric (glass-like) material
class dielectric : public material
{
    public:
        dielectric(float ri) : ref_idx(ri) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const override
        {
            vec3 outward_normal;
            vec3 reflected = reflect(r_in.Direction(), rec.normal);
            float ni_over_nt;
            attenuation = vec3(1.0, 1.0, 1.0); // no attenuation for dielectric
            vec3 refracted;
            float reflect_prob;
            float cosine;

            if (dot(r_in.Direction(), rec.normal) > 0)
            {
                outward_normal = -rec.normal;
                ni_over_nt = ref_idx;
                cosine = ref_idx * dot(r_in.Direction(), rec.normal) / r_in.Direction().length();
            }
            else
            {
                outward_normal = rec.normal;
                ni_over_nt = 1.0 / ref_idx;
                cosine = -dot(r_in.Direction(), rec.normal) / r_in.Direction().length();
            }

            if (refract(r_in.Direction(), outward_normal, ni_over_nt, refracted))
            {
                reflect_prob = schlick(cosine, ref_idx);
            }
            else
            {
                reflect_prob = 1.0;
            }

            if (random_float() < reflect_prob)
            {
                scattered = Ray(rec.p, reflected);
            }
            else
            {
                scattered = Ray(rec.p, refracted);
            }

            return true;
        }

        float ref_idx;

    private:
        vec3 reflect(const vec3& v, const vec3& n) const
        {
            return v - 2 * dot(v, n) * n;
        };

        bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) const
        {
            vec3 uv = v.unit_vector();
            float dt = dot(uv, n);
            float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
            if (discriminant > 0)
            {
                refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
                return true;
            }
            else
            {
                return false;
            }
        };

        float schlick(float cosine, float ref_idx) const
        {
            float r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        };
};

#endif