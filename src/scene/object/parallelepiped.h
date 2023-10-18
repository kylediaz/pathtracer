#ifndef PP_H
#define PP_H

#include "object.h"
#include "material.h"

#include "quad.h"

namespace scene
{

    class Material;

    class Parallelepiped : public HittableGroup
    {
    public:
        Parallelepiped(const Point3 &a, const Point3 &b, shared_ptr<Material> mat)
        {
            // Construct the two opposite vertices with the minimum and maximum coordinates.
            auto min = Point3(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
            auto max = Point3(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

            auto dx = Vec3(max.x() - min.x(), 0, 0);
            auto dy = Vec3(0, max.y() - min.y(), 0);
            auto dz = Vec3(0, 0, max.z() - min.z());

            add(make_shared<quad>(Point3(min.x(), min.y(), max.z()), dx, dy, mat));  // front
            add(make_shared<quad>(Point3(max.x(), min.y(), max.z()), -dz, dy, mat)); // right
            add(make_shared<quad>(Point3(max.x(), min.y(), min.z()), -dx, dy, mat)); // back
            add(make_shared<quad>(Point3(min.x(), min.y(), min.z()), dz, dy, mat));  // left
            add(make_shared<quad>(Point3(min.x(), max.y(), max.z()), dx, -dz, mat)); // top
            add(make_shared<quad>(Point3(min.x(), min.y(), min.z()), dx, dz, mat));  // bottom
        }
    };

}

#endif