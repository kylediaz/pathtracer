#ifndef SPHERE_H
#define SPHERE_H

#include "./ptmath/vec3.h"
#include "./ptmath/interval.h"

#include "object.h"

namespace scene
{

    class Material;

    class sphere : public Hittable
    {
    public:
        sphere(Point3 _center, double _radius, shared_ptr<Material> _material)
            : center(_center), radius(_radius), mat(_material) {}

        bool hit(const ray &r, interval ray_t, HitRecord &rec) const override
        {
            Vec3 oc = r.origin() - center;
            auto a = r.direction().length_squared();
            auto half_b = dot(oc, r.direction());
            auto c = oc.length_squared() - radius * radius;

            auto discriminant = half_b * half_b - a * c;
            if (discriminant < 0)
                return false;
            auto sqrtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            auto root = (-half_b - sqrtd) / a;
            if (!ray_t.surrounds(root))
            {
                root = (-half_b + sqrtd) / a;
                if (!ray_t.surrounds(root))
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat = mat;
            rec.set_face_normal(r, rec.normal);

            return true;
        }

    private:
        Point3 center;
        double radius;
        shared_ptr<Material> mat;
    };

}

#endif