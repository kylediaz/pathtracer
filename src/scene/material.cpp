#include "./geometry/vec3.h"
#include "material.h"

bool lambertian::scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
    const
{
    auto scatter_direction = rec.normal + random_unit_vector();
    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
}

bool metal::scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
    const
{
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return true;
}

bool dielectric::scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
    const
{
    attenuation = .9 * color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    vec3 unit_direction = unit_vector(r_in.direction());
    vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

    scattered = ray(rec.p, refracted);
    return true;
}