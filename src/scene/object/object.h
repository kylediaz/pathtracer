#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

using namespace ptmath;

#include "./ptmath/ray.h"
#include "./ptmath/interval.h"

namespace scene
{

    class Material;

    class HitRecord
    {
    public:
        Point3 p;
        Vec3 normal;
        double t;
        shared_ptr<Material> mat;
        bool front_face;

        void set_face_normal(const ray &r, const Vec3 &outward_normal)
        {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };

    class Hittable
    {
    public:
        virtual ~Hittable() = default;

        virtual bool hit(const ray &r, interval ray_t, HitRecord &rec) const = 0;
    };

    class HittableGroup : public Hittable
    {
    public:
        std::vector<shared_ptr<Hittable>> objects;

        HittableGroup() {}
        HittableGroup(shared_ptr<Hittable> object) { add(object); }

        void clear() { objects.clear(); }

        void add(shared_ptr<Hittable> object)
        {
            objects.push_back(object);
        }

        bool hit(const ray &r, interval ray_t, HitRecord &rec) const override
        {
            HitRecord temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            for (const auto &object : objects)
            {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
                {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }
    };

}

#endif