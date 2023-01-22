#ifndef TREASACH_SCREEN_H
#define TREASACH_SCREEN_H

#include <vector>
#include <cinttypes>
#include "../geometry/Vector3.h"


class ContinuousColour {
    public:
        double red   = 0;
        double green = 0;
        double blue  = 0;

        ContinuousColour operator+(const ContinuousColour& other) const;
};

ContinuousColour operator*(double scalar, const ContinuousColour& c);



struct Pixel {
    unsigned x;
    unsigned y;

    ContinuousColour c;
};


class Screen {
    public:
        double stride = 1;
        unsigned width;
        unsigned height;
        std::vector<Pixel> pixels;

        Screen(unsigned width, unsigned height);

    private:
};


#endif //TREASACH_SCREEN_H
