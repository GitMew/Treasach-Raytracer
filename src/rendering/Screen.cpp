#include "Screen.h"


Screen::Screen(unsigned int width, unsigned int height)
    : width(width), height(height) {
    for (unsigned x = 0; x < this->width; x++) {
        for (unsigned y = 0; y < this->height; y++) {
            pixels.push_back(Pixel({x, y}));
        }
    }
}


ContinuousColour ContinuousColour::operator+(const ContinuousColour &other) const {
    return {
        this->red   + other.red,
        this->green + other.green,
        this->blue  + other.blue
    };
}

ContinuousColour operator*(double scalar, const ContinuousColour& c) const {
    return {
        scalar*c.red,
        scalar*c.green,
        scalar*c.blue
    }
}
