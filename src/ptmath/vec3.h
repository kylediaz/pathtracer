#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "./util/util.h"

namespace ptmath
{

    class Vec3
    {
    public:
        double e[3];

        Vec3() : e{0, 0, 0} {}
        Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        double &operator[](int i) { return e[i]; }

        Vec3 &operator+=(const Vec3 &v)
        {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        Vec3 &operator*=(double t)
        {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        Vec3 &operator/=(double t)
        {
            return *this *= 1 / t;
        }

        double length() const
        {
            return std::sqrt(length_squared());
        }

        double length_squared() const
        {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        bool near_zero() const
        {
            auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }

        static Vec3 random()
        {
            return Vec3(util::RandomDouble(), util::RandomDouble(), util::RandomDouble());
        }

        static Vec3 random(double min, double max)
        {
            return Vec3(util::RandomDouble(min, max), util::RandomDouble(min, max), util::RandomDouble(min, max));
        }
    };

    // Point3 is just an alias for vec3, but useful for geometric clarity in the code.
    using Point3 = Vec3;

    // Vector Utility Functions

    inline std::ostream &operator<<(std::ostream &out, const Vec3 &v)
    {
        return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    }

    inline Vec3 operator+(const Vec3 &u, const Vec3 &v)
    {
        return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
    }

    inline Vec3 operator-(const Vec3 &u, const Vec3 &v)
    {
        return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
    }

    inline Vec3 operator*(const Vec3 &u, const Vec3 &v)
    {
        return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
    }

    inline Vec3 operator*(double t, const Vec3 &v)
    {
        return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
    }

    inline Vec3 operator*(const Vec3 &v, double t)
    {
        return t * v;
    }

    inline Vec3 operator/(Vec3 v, double t)
    {
        return (1 / t) * v;
    }

    inline Vec3 sqrt(Vec3 v)
    {
        return Vec3(std::sqrt(v.e[0]), std::sqrt(v.e[1]), std::sqrt(v.e[2]));
    }

    inline double dot(const Vec3 &u, const Vec3 &v)
    {
        return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
    }

    inline Vec3 cross(const Vec3 &u, const Vec3 &v)
    {
        return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                    u.e[2] * v.e[0] - u.e[0] * v.e[2],
                    u.e[0] * v.e[1] - u.e[1] * v.e[0]);
    }

    inline Vec3 reflect(const Vec3 &v, const Vec3 &n)
    {
        return v - 2 * dot(v, n) * n;
    }

    inline Vec3 refract(const Vec3 &uv, const Vec3 &n, double etai_over_etat)
    {
        auto cos_theta = fmin(dot(-uv, n), 1.0);
        Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
        Vec3 r_out_parallel = -std::sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
        return r_out_perp + r_out_parallel;
    }

    inline Vec3 unit_vector(Vec3 v)
    {
        return v / v.length();
    }

    inline auto distance2(const Vec3 &u, const Vec3 &v)
    {
        const double dx = u.x() - v.x();
        const double dy = u.y() - v.y();
        const double dz = u.z() - v.y();
        return dx * dx + dy * dy + dz * dz;
    }

    inline Vec3 random_unit_vector()
    {
        return unit_vector(Vec3::random());
    }

    inline Vec3 random_on_hemisphere(const Vec3 &normal)
    {
        Vec3 on_unit_sphere = random_unit_vector();
        if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
            return on_unit_sphere;
        else
            return -on_unit_sphere;
    }

}

#endif