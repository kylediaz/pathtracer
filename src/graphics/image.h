#ifndef IMAGE_H
#define IMAGE_H

#include <thread>
#include <iostream>

#include "color.h"

class image
{
public:
    image(int width, int height) : w(width), h(height)
    {
        b = new color[width * height];
    }

    int width() const { return w; }
    int height() const { return h; }
    color *buffer() const { return b; }

    void flushToPPM()
    {
        std::cout << "P3\n"
                  << w << ' ' << h << "\n255\n";

        for (int i = 0; i < w * h; i++)
        {
            color pixel_color = sqrt(b[i]);
            std::cout << static_cast<int>(255.999 * pixel_color.x()) << ' '
                      << static_cast<int>(255.999 * pixel_color.y()) << ' '
                      << static_cast<int>(255.999 * pixel_color.z()) << '\n';
        }
    }

private:
    int w;
    int h;
    color *b;
};

#endif