#include "Colour.h"

ContinuousColour ContinuousColour::operator+(const ContinuousColour &other) const {
    return {
            this->red   + other.red,
            this->green + other.green,
            this->blue  + other.blue
    };
}

void ContinuousColour::operator+=(const ContinuousColour& other) {
    this->red   += other.red;
    this->green += other.green;
    this->blue  += other.blue;
}

ContinuousColour operator*(double scalar, const ContinuousColour& c) {
    return {
            scalar*c.red,
            scalar*c.green,
            scalar*c.blue
    };
}

std::ostream& operator<<(std::ostream& os, const ContinuousColour& c) {
    return os << "(" << c.red << ", " << c.green << ", " << c.blue << ")";
}
