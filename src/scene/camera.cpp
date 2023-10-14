#include "camera.h"

#include "./util/util.h"
#include "./graphics/image.h"
#include "object/object.h"
#include "material.h"

void camera::render(const hittable_list &world, int cores)
{
    if (cores <= 0)
    {
        return;
    }

    this->initialize();

    image output(image_width, image_height);

    int pixel_index = 0;
    for (int j = 0; j < image_height; ++j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            color c = render_pixel(world, i, j);
            output.buffer()[pixel_index++] = c;
        }
    }

    output.flushToPPM();
}
void camera::render_scanline(const hittable_list &world, const image &output, const int line_start, const int line_end)
{
    int pixel_index = 0;
    for (int j = 0; j < image_height; ++j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            color c = render_pixel(world, i, j);
            output.buffer()[pixel_index++] = c;
        }
    }
}
color camera::render_pixel(const hittable_list &world, int i, int j)
{
    color color;
    for (int sample = 0; sample < samples_per_pixel; sample++)
    {
        double u_variance = random_double() - .5;
        double v_variance = random_double() - .5;

        auto vj = V * (double(j) + u_variance) / (image_height - 1);
        auto ui = U * (double(i) + v_variance) / (image_width - 1);
        auto vp = viewport_upper_left + vj + ui;

        auto r = ray(center, vp);
        color += ray_color(r, world);
    }
    color = color / samples_per_pixel;
    return color;
}

void camera::initialize()
{
    auto focal_length = 1.0;
    auto viewport_height = 2.0;

    aspect_ratio = 1. * image_width / image_height;
    auto viewport_width = viewport_height * aspect_ratio;

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    U = viewport_width * u;
    V = -viewport_height * v;

    viewport_upper_left = center - (focal_length * w) - U/2 - V/2;
    viewport_center = viewport_upper_left + .5 * (U + V);
}

color camera::ray_color(const ray &r, const hittable_list &world, const int depth)
{
    if (depth <= 0)
    {
        return color(0, 0, 0);
    }

    hit_record rec;
    if (world.hit(r, interval(0.001, INFINITY), rec))
    {
        if (rec.mat == NULL) // Default material
        {
            vec3 direction = rec.normal + random_unit_vector();
            return 0.7 * ray_color(ray(rec.p, direction), world, depth - 1);
        }

        ray scattered;
        color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}