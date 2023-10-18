#include "ptmath/vec3.h"
#include "graphics/color.h"
#include "ptmath/ray.h"
#include "ptmath/tri3.h"

#include "scene/object/object.h"
#include "scene/object/sphere.h"
#include "scene/object/tri.h"
#include "scene/object/quad.h"
#include "scene/object/parallelepiped.h"
#include "scene/material.h"

#include "scene/camera.h"

#include <iostream>
#include <chrono>

using namespace ptmath;
using namespace scene;

void Weekend(HittableGroup &world, Camera &cam)
{
    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = util::RandomDouble();
            Point3 center(a + 0.9 * util::RandomDouble(), 0.2, b + 0.9 * util::RandomDouble());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    sphere_material = make_shared<Metal>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5));

    world.add(make_shared<sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<CheckeredLambertian>(.1, color(0, 0, 0), color(1, 0, 1));
    world.add(make_shared<sphere>(Point3(-4, 1, 0), 1.0, material2));

    world.add(make_shared<sphere>(Point3(4, 1, 0), 1.0, material3));

    cam.vfov_ = 20;
    cam.look_from_ = Point3(13, 2, 3);
    cam.lookat_ = Point3(0, 0, 0);
    cam.vup_ = Vec3(0, 1, 0);
}

void Quads(HittableGroup &world, Camera &cam)
{
    // Materials
    auto left_red = make_shared<Lambertian>(color(1.0, 0.2, 0.2));
    auto back_green = make_shared<Lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue = make_shared<Lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<Lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<Lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(Point3(-3, -2, 5), Vec3(0, 0, -4), Vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(Point3(-2, -2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(Point3(3, -2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(Point3(-2, -3, 5), Vec3(4, 0, 0), Vec3(0, 0, -4), lower_teal));

    cam.vfov_ = 80;
    cam.look_from_ = Point3(0, 0, 9);
    cam.lookat_ = Point3(0, 0, 0);
    cam.vup_ = Vec3(0, 1, 0);
}

void Room(HittableGroup &world, Camera &cam)
{
    auto mirror = make_shared<Lambertian>(color(0.7, 0.6, 0.5));
    auto light = make_shared<Light>(color(1, 1, 1));
    auto red = make_shared<Lambertian>(color(1, 0, 0));

    double box_scale = 4;

    world.add(make_shared<Parallelepiped>(box_scale * Point3(-1, -1, -1), box_scale * Point3(1, 1, 1), mirror));
    world.add(make_shared<Parallelepiped>((box_scale / 4) * Point3(-1, -1, -1) + Vec3(0, box_scale * 1.2, 0),
                                          (box_scale / 4) * Point3(1, 1, 1) + Vec3(0, box_scale * 1.2, 0), light));

    world.add(make_shared<sphere>(Point3(0, 0, 0), 1.0, red));

    cam.look_from_ = Point3(0, 0, .99 * box_scale);
    cam.lookat_ = Point3(0, 0, 0);
}

void CornellBox(HittableGroup &world, Camera &cam) {
    auto red   = make_shared<Lambertian>(color(.65, .05, .05));
    auto white = make_shared<Lambertian>(color(.73, .73, .73));
    auto green = make_shared<Lambertian>(color(.12, .45, .15));
    auto light = make_shared<Light>(color(15, 15, 15));

    world.add(make_shared<quad>(Point3(555,0,0), Vec3(0,555,0), Vec3(0,0,555), green));
    world.add(make_shared<quad>(Point3(0,0,0), Vec3(0,555,0), Vec3(0,0,555), red));
    world.add(make_shared<quad>(Point3(343, 554, 332), Vec3(-130,0,0), Vec3(0,0,-105), light));
    world.add(make_shared<quad>(Point3(0,0,0), Vec3(555,0,0), Vec3(0,0,555), white));
    world.add(make_shared<quad>(Point3(555,555,555), Vec3(-555,0,0), Vec3(0,0,-555), white));
    world.add(make_shared<quad>(Point3(0,0,555), Vec3(555,0,0), Vec3(0,555,0), white));

    cam.vfov_     = 40;
    cam.look_from_ = Point3(278, 278, -800);
    cam.lookat_   = Point3(278, 278, 0);
    cam.vup_      = Vec3(0,1,0);
}

int main()
{
    int num_cores = 4;

    HittableGroup world;

    BatchedMultiThreadCamera cam;

    cam.image_height_ = 1080 / 4;
    cam.image_width_ = 1920 / 4;
    cam.samples_per_pixel_ = 10;
    cam.max_depth_ = 5;

    CornellBox(world, cam);

    auto start = std::chrono::high_resolution_clock::now();
    cam.Render(world, num_cores);
    auto stop = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    std::clog << "Total time: " << (ns / 1e6) << "\n";
}
