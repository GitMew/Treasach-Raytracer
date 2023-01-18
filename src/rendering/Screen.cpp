#include "Screen.h"

Screen::Screen(unsigned int width, unsigned int height)
    : width(width), height(height) {}


std::vector<Pixel> Screen::generatePixels() const {
    auto pixels = std::vector<Pixel>();
    for (unsigned x = 0; x < this->width; x++) {
        for (unsigned y = 0; y < this->height; y++) {
            pixels.push_back(Pixel({x, y}));
        }
    }

    return pixels;
}