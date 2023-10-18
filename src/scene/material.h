#ifndef MATERIAL_H
#define MATERIAL_H

#include "./ptmath/vec3.h"
#include "./ptmath/ray.h"
#include "./graphics/color.h"
#include "object/object.h"

using namespace ptmath;

namespace scene
{

    class Material
    {
    public:
        virtual ~Material() = default;

        virtual bool Scatter(
            const ray &r_in, const HitRecord &rec, color &attenuation, ray &scattered) const = 0;

        virtual color Emit(
            [[maybe_unused]] const ray &r_in, [[maybe_unused]] const HitRecord &rec) const
        {
            return color(0, 0, 0);
        }
    };

    // Solid Materials

    class Lambertian : public Material
    {
    public:
        Lambertian(const color &a) : albedo_(a) {}
        bool Scatter(const ray &r_in, const HitRecord &rec, color &attenuation, ray &scattered)
            const override;

    private:
        color albedo_;
    };

    class CheckeredLambertian : public Material
    {
    public:
        CheckeredLambertian(const double scale, const color &c1, const color &c2) : scale_(scale), albedo_1_(c1), albedo_2_(c2) {}
        bool Scatter(const ray &r_in, const HitRecord &rec, color &attenuation, ray &scattered)
            const override;

    private:
        double scale_;
        color albedo_1_, albedo_2_;
    };

    // Special Properties

    class Metal : public Material
    {
    public:
        Metal(const color &a) : albedo_(a) {}

        bool Scatter(const ray &r_in, const HitRecord &rec, color &attenuation, ray &scattered)
            const override;

    private:
        color albedo_;
    };

    class Dielectric : public Material
    {
    public:
        Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        bool Scatter(const ray &r_in, const HitRecord &rec, color &attenuation, ray &scattered)
            const override;

    private:
        double ir; // Index of Refraction
    };

    class Light : public Material
    {
    public:
        Light(const color &a) : albedo_(a) {}

        bool Scatter(const ray &r_in, const HitRecord &rec, color &attenuation, ray &scattered)
            const override;
        color Emit(const ray &r_in, const HitRecord &rec) const override;

    private:
        color albedo_;
    };

}

#endif