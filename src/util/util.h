#ifndef UTIL_H
#define UTIL_H

#define kPi 3.14159
#define kEpsilon .0001

#include <random>

namespace util
{

    inline double RandomDouble()
    {
        // Returns a random real in [0,1).
        return rand() / (RAND_MAX + 1.0);
    }

    inline double RandomDouble(double min, double max)
    {
        // Returns a random real in [min,max).
        return min + (max - min) * RandomDouble();
    }

    inline double DegreesToRadians(double degrees)
    {
        return degrees * (kPi / 180);
    }

    void PrintProgress(double percentage);

};

#endif