#ifndef QUAD_H
#define QUAD_H

#include "object.h"

using namespace ptmath;

namespace scene
{

    class quad : public Hittable
    {
    public:
        quad(const Point3 &_Q, const Vec3 &_u, const Vec3 &_v, shared_ptr<Material> m)
            : Q(_Q), u(_u), v(_v), mat(m)
        {
            auto n = cross(u, v);
            normal = unit_vector(n);
            D = dot(normal, Q);
            w = n / dot(n, n);
        }

        bool hit(const ray &r, interval ray_t, HitRecord &rec) const override
        {
            auto denom = dot(normal, r.direction());

            // No hit if the ray is parallel to the plane.
            if (fabs(denom) < 1e-8)
                return false;

            // Return false if the hit point parameter t is outside the ray interval.
            auto t = (D - dot(normal, r.origin())) / denom;
            if (!ray_t.contains(t))
                return false;

            // Determine the hit point lies within the planar shape using its plane coordinates.
            auto intersection = r.at(t);
            Vec3 planar_hitpt_vector = intersection - Q;
            auto alpha = dot(w, cross(planar_hitpt_vector, v));
            auto beta = dot(w, cross(u, planar_hitpt_vector));

            if (!is_interior(alpha, beta, rec))
                return false;

            // Ray hits the 2D shape; set the rest of the hit record and return true.
            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);

            return true;
        }

        virtual bool is_interior(double a, double b, HitRecord &rec) const
        {
            // Given the hit point in plane coordinates, return false if it is outside the
            // primitive, otherwise set the hit record UV coordinates and return true.

            if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
                return false;

            //rec.u = a;
            //rec.v = b;
            return true;
        }

    private:
        Point3 Q;
        Vec3 u, v;
        shared_ptr<Material> mat;
        Vec3 normal;
        double D;
        Vec3 w;
    };

}

#endif