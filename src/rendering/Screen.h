#ifndef TREASACH_SCREEN_H
#define TREASACH_SCREEN_H

#include <vector>
#include <cinttypes>

struct Colour {
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
};


struct Pixel {
    unsigned x;
    unsigned y;

    Colour c;
};


class Screen {
    public:
        Screen(unsigned width, unsigned height);

        std::vector<Pixel> generatePixels() const;  // Should this be an iterator?

    private:
        unsigned width;
        unsigned height;
};


#endif //TREASACH_SCREEN_H
