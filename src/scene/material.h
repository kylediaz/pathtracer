#ifndef MATERIAL_H
#define MATERIAL_H

#include "./geometry/vec3.h"
#include "./geometry/ray.h"
#include "./graphics/color.h"
#include "object/object.h"

class material
{
public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const color &a) : albedo(a) {}
    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override;

private:
    color albedo;
};

class metal : public material
{
public:
    metal(const color &a) : albedo(a) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override;

private:
    color albedo;
};


class dielectric : public material {
  public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override;

  private:
    double ir; // Index of Refraction
};

static inline color default_color = color(1, 0, 0);

#endif