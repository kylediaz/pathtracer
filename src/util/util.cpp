#include "util.h"

#include <iostream>
#include <string>

using namespace util;

void util::PrintProgress(double percentage)
{
    const int kPLENGTH = 20;
    std::clog << "\r[" 
            << std::string(kPLENGTH * percentage, 'X') 
            << std::string(kPLENGTH * (1 - percentage), '-') 
            << "] " << 100 * percentage << "% " << std::flush;
}
