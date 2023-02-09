#include "Screen.h"


Screen::Screen(unsigned int width, unsigned int height)
    : width(width), height(height) {
    for (unsigned y = 0; y < this->height; y++) {
        for (unsigned x = 0; x < this->width; x++) {
            pixels.push_back(Pixel({x, y}));
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Pixel& p) {
    return os << "(" << p.x << ", " << p.y << ") " << p.radiance;
}