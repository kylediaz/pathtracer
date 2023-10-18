#ifndef CAMERA_H
#define CAMERA_H

#include <thread>
#include <iostream>
#include <mutex>

#include "./ptmath/vec3.h"
#include "./graphics/color.h"
#include "./graphics/image.h"
#include "object/object.h"

using namespace ptmath;

namespace scene
{

    class Camera
    {
    public:
        int image_width_ = 400;
        int image_height_ = 200;

        int samples_per_pixel_ = 10;
        int max_depth_ = 10;

        double vfov_ = 90;

        Point3 look_from_ = Point3(0, 0, -1); // Point camera is looking from
        Point3 lookat_ = Point3(0, 0, 0);     // Point camera is looking at
        Vec3 vup_ = Vec3(0, 1, 0);            // Camera-relative "up" direction

        void Render(const hittable_list &world);

    protected:
        double aspect_ratio_;

        Point3 center;
        Vec3 u, v, w;
        Vec3 U, V;
        Point3 viewport_upper_left;
        Point3 viewport_center;

        void Initialize();

        color RenderPixel(const hittable_list &world, int i, int j);

        ray GetRayForPixel(const int i, const int j);

        color RenderRay(const ray &r, const hittable_list &world)
        {
            return RenderRay(r, world, max_depth_);
        }
        color RenderRay(const ray &r, const hittable_list &world, const int depth);

    };

    /**
     * Threads take threads as they go until all lines are rendered
    */
    class MultiThreadCamera : public Camera
    {
    public:
        void Render(const hittable_list &world, const int num_threads);
    private:
        void ThreadJob(const hittable_list &world, const image &output, int& line_ref, std::mutex& mu);
    protected:
        void RenderScanline(const hittable_list &world, const image &output, const int line);
    };

    /**
     * Each thread is assigned a sector of the image to render at the beginning
    */
    class BatchedMultiThreadCamera : public MultiThreadCamera
    {
    public:
        void Render(const hittable_list &world, const int num_threads);

    protected:
        void RenderScanlines(const hittable_list &world, const image &output, const int line_start, const int line_end);
    };

}

#endif