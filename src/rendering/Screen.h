#ifndef TREASACH_SCREEN_H
#define TREASACH_SCREEN_H

#include <vector>
#include <cinttypes>
#include "../geometry/Vector3.h"
#include "../shading/Colour.h"

struct Pixel {
    unsigned x;
    unsigned y;

    ContinuousColour radiance;
};

std::ostream& operator<<(std::ostream& os, const Pixel& p);


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
