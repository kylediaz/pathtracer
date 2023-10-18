#ifndef TRI_H
#define TRI_H

#include "./ptmath/vec3.h"
#include "./ptmath/tri3.h"
#include "./ptmath/interval.h"

#include "object.h"

namespace scene
{

    class Material;

    class Tri : public Hittable
    {
    public:
        Tri(Tri3 tri, shared_ptr<Material> _material)
            : tri_(tri), mat(_material) {}

        bool hit(const ray &r, interval ray_t, HitRecord &rec) const override
        {
            int t;
            if (!tri_.intersect(r, t) || !ray_t.contains(t)) {
                return false;
            }

            rec.t = t;
            rec.p = r.at(rec.t);
            rec.normal = tri_.normal();
            rec.mat = mat;
            rec.set_face_normal(r, rec.normal);

            return true;
        }

    private:
        Tri3 tri_;
        shared_ptr<Material> mat;
    };

}

#endif