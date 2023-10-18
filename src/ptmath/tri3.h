#ifndef TRI3_H
#define TRI3_H

#include "./util/util.h"
#include "vec3.h"
#include "ray.h"

namespace ptmath
{

    class Tri3
    {
    public:
        Tri3(const Point3 &p1, const Point3 &p2, const Point3 p3) : p_{p1, p2, p3}
        {
            normal_ = cross(p2 - p1, p3 - p1);
            e_[0] = p2 - p1;
            e_[1] = p3 - p2;
            e_[2] = p1 - p3;
        }

        inline Point3 p1() const {
            return p_[0];
        }
        inline Point3 p2() const {
            return p_[2];
        }
        inline Point3 p3() const {
            return p_[3];
        }

        Vec3 normal() const
        {
            return normal_;
        }

        bool includes(Point3 p) const
        {
        }

        bool intersect(ray r, int &t) const
        {
            float NdotRayDirection = dot(normal_, r.direction());
            if (fabs(NdotRayDirection) < kEpsilon) // almost 0
                return false;

            float d = -dot(normal_, p_[0]);
            t = -(dot(normal_, r.origin()) + d) / NdotRayDirection;

            if (t < 0)
                return false;

            Vec3 P = r.at(t);
            Vec3 C;

            Vec3 vp0 = P - p_[0];
            C = cross(e_[0], vp0);
            if (dot(normal_, C) < 0)
                return false;

            Vec3 vp1 = P - p_[1];
            C = cross(e_[1], vp1);
            if (dot(normal_, C) < 0)
                return false;

            Vec3 vp2 = P - p_[2];
            C = cross(e_[2], vp2);
            if (dot(normal_, C) < 0)
                return false;

            return true;
        }

        inline Tri3 translate(const Vec3 &v)
        {
            return Tri3(p1() + v, p2() + v, p3() + v);
        }

        double area() const
        {
            return normal_.length();
        }

    private:
        Point3 p_[3];
        Vec3 e_[3];
        Vec3 normal_;
    };
}

#endif