#include "camera.h"

#include "./util/util.h"
#include "./graphics/image.h"
#include "object/object.h"
#include "material.h"

#include <math.h>
#include <mutex>

using namespace scene;
using namespace ptmath;

void Camera::Initialize()
{
    center = look_from_;

    aspect_ratio_ = 1. * image_width_ / image_height_;

    auto focal_length = (look_from_ - lookat_).length();
    auto theta = util::DegreesToRadians(vfov_);
    auto h = tan(theta / 2);
    auto viewport_height = 2 * h * focal_length;
    auto viewport_width = viewport_height * aspect_ratio_;

    w = unit_vector(look_from_ - lookat_);
    u = unit_vector(cross(vup_, w));
    v = cross(w, u);

    U = viewport_width * u;
    V = viewport_height * -v;

    viewport_center = center - focal_length * w;
    viewport_upper_left = center - (focal_length * w) - U / 2 - V / 2;
    std::clog << center;
}

void Camera::Render(const HittableGroup &world)
{
    this->Initialize();

    image output(image_width_, image_height_);

    int pixel_index = 0;
    for (int j = 0; j < image_height_; ++j)
    {
        for (int i = 0; i < image_width_; ++i)
        {
            color c = RenderPixel(world, i, j);
            output.buffer()[pixel_index++] = c;
        }
        double progress = (double) j / image_height_;
        util::PrintProgress(progress);
    }

    output.flushToPPM();
}

color Camera::RenderPixel(const HittableGroup &world, int i, int j)
{
    color color;
    for (int sample = 0; sample < samples_per_pixel_; sample++)
    {
        ray r = GetRayForPixel(i, j);
        color += RenderRay(r, world);
    }
    color = color / samples_per_pixel_;
    return color;
}

ray Camera::GetRayForPixel(const int i, const int j)
{
    double u_variance = util::RandomDouble() - .5;
    double v_variance = util::RandomDouble() - .5;

    auto vj = V * (double(j) + u_variance) / (image_height_ - 1);
    auto ui = U * (double(i) + v_variance) / (image_width_ - 1);
    auto vp = viewport_upper_left + vj + ui;

    Point3 origin = center;
    Vec3 direction = vp - origin;
    return ray(origin, direction);
}

color Camera::RenderRay(const ray &r, const HittableGroup &world, const int depth)
{
    if (depth <= 0)
    {
        return color(0, 0, 0);
    }

    HitRecord rec;
    if (world.hit(r, interval(0.001, INFINITY), rec))
    {
        if (rec.mat == NULL) // Default material
        {
            Vec3 direction = rec.normal + random_unit_vector();
            return 0.7 * RenderRay(ray(rec.p, direction), world, depth - 1);
        }

        ray scattered;
        color attenuation;
        if (rec.mat->Scatter(r, rec, attenuation, scattered))
            return attenuation * RenderRay(scattered, world, depth - 1);
        return rec.mat->Emit(r, rec);
    }

    Vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

// Multi Threaded

void MultiThreadCamera::Render(const HittableGroup &world, int num_threads)
{
    if (num_threads <= 0)
    {
        return;
    }

    this->Initialize();

    image output(image_width_, image_height_);

    std::vector<std::thread> threads;
    std::mutex mu;
    int line = 0;

    for (int t = 0; t < num_threads; t++)
    {
        threads.emplace_back(&BatchedMultiThreadCamera::ThreadJob, *this, world, output, std::ref(line), std::ref(mu));
    }

    // Join the threads to wait for them to finish
    for (std::thread &thread : threads)
    {
        thread.join();
    }

    output.flushToPPM();
}

void MultiThreadCamera::ThreadJob(const HittableGroup &world, const image &output, int& line_ref, std::mutex& mu) {
    int current_line = -1;
    while (current_line < image_height_) {
        mu.lock();
        current_line = line_ref;
        line_ref += 1;
        mu.unlock();
        if (current_line < image_height_) {
            RenderScanline(world, output, current_line);
        }
        double progress = (double) current_line / image_height_;
        util::PrintProgress(progress);
    }
}

void MultiThreadCamera::RenderScanline(const HittableGroup &world, const image &output, const int line)
{
    int pixel_index = line * image_width_;
    for (int x = 0; x < image_width_; ++x)
    {
        color c = RenderPixel(world, x, line);
        output.buffer()[pixel_index++] = c;
    }
}

void BatchedMultiThreadCamera::Render(const HittableGroup &world, int num_threads)
{
    if (num_threads <= 0)
    {
        return;
    }

    this->Initialize();

    image output(image_width_, image_height_);

    std::vector<std::thread> threads;

    int rangeSize = image_height_ / num_threads;
    for (int i = 0; i < num_threads; i++)
    {
        int start = i * rangeSize;
        int end = (i + 1) * rangeSize - 1;
        threads.emplace_back(&BatchedMultiThreadCamera::RenderScanlines, *this, world, output, start, end);
    }
    if (image_height_ % rangeSize != 0)
    {
        int start = image_height_ - (image_height_ % rangeSize);
        int end = image_height_ - 1;
        threads.emplace_back(&BatchedMultiThreadCamera::RenderScanlines, *this, world, output, start, end);
    }

    // Join the threads to wait for them to finish
    for (std::thread &thread : threads)
    {
        thread.join();
    }

    output.flushToPPM();
}

void BatchedMultiThreadCamera::RenderScanlines(const HittableGroup &world, const image &output, const int line_start, const int line_end)
{
    for (int y = line_start; y <= line_end; ++y)
    {
        RenderScanline(world, output, y);
        double progress = (double)(y - line_start) / (line_end - line_start);
        util::PrintProgress(progress);
    }
}