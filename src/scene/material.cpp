#include "./ptmath/vec3.h"
#include "material.h"

using namespace scene;
using namespace ptmath;

bool Lambertian::Scatter(const ray &r_in, const HitRecord &rec, color &attenuation, ray &scattered)
    const
{
    auto scatter_direction = rec.normal + random_unit_vector();
    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo_;
    return true;
}

bool CheckeredLambertian::Scatter(const ray &r_in, const HitRecord &rec, color &attenuation, ray &scattered)
    const
{
    auto scatter_direction = rec.normal + random_unit_vector();
    scattered = ray(rec.p, scatter_direction);

    Point3 p = (1 / scale_) * rec.p;
    auto sum = ((int) p.x() + (int) p.y() + (int) p.z());
    attenuation = sum % 2 != 0 ? albedo_1_ : albedo_2_;

    return true;
}

bool Metal::Scatter(const ray &r_in, const HitRecord &rec, color &attenuation, ray &scattered)
    const
{
    Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected);
    attenuation = albedo_;
    return true;
}

bool Dielectric::Scatter(const ray &r_in, const HitRecord &rec, color &attenuation, ray &scattered)
    const
{
    attenuation = .9 * color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    Vec3 unit_direction = unit_vector(r_in.direction());
    Vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

    scattered = ray(rec.p, refracted);
    return true;
}

bool Light::Scatter(const ray &r_in, const HitRecord &rec, color &attenuation, ray &scattered)
    const
{
    return false;
}

color Light::Emit(const ray &r_in, const HitRecord &rec) const {
    return albedo_;
}