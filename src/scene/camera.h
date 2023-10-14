#ifndef CAMERA_H
#define CAMERA_H

#include <thread>
#include <iostream>

#include "./geometry/vec3.h"
#include "./graphics/color.h"
#include "./graphics/image.h"
#include "object/object.h"

class camera
{
public:
    int image_width = 400;
    int image_height = 200;

    int samples_per_pixel = 10;
    int max_depth = 10;

    point3 lookfrom = point3(0,0,-1);  // Point camera is looking from
    point3 lookat   = point3(0,0,0);   // Point camera is looking at
    vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction

    void render(const hittable_list &world)
    {
        render(world, max_depth);
    }

    void render(const hittable_list &world, const int cores);

    color render_pixel(const hittable_list &world, int i, int j);

private:
    double aspect_ratio;

    point3 center;
    vec3 u, v, w;
    vec3 U, V;
    point3 viewport_upper_left;
    point3 viewport_center;

    void initialize();

    std::thread thread_job(const hittable_list &world, const image &output, const int line_start, const int line_end);
    void render_scanline(const hittable_list &world, const image &output, const int line_start, const int line_end);

    color ray_color(const ray &r, const hittable_list &world)
    {
        return ray_color(r, world, max_depth);
    }
    color ray_color(const ray &r, const hittable_list &world, const int depth);
};

#endif