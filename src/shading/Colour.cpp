#include "Colour.h"

ContinuousColour::ContinuousColour(double red, double green, double blue)
    : red(red), green(green), blue(blue) {}

ContinuousColour::ContinuousColour()
        : ContinuousColour(0,0,0) {}


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

ContinuousColour ContinuousColour::L1normalised() const {
    return 1.0/(this->red + this->green + this->blue) * (*this);
}

ContinuousColour operator*(const ContinuousColour& c1, const ContinuousColour& c2) {
    return {
        c1.red * c2.red,
        c1.green * c2.green,
        c1.blue * c2.blue
    };
}

ContinuousColour operator*(double scalar, const ContinuousColour& c) {
    return {
            scalar*c.red,
            scalar*c.green,
            scalar*c.blue
    };
}

ContinuousColour operator*(const ContinuousColour& c, double scalar) {
    return scalar * c;
}

ContinuousColour operator/(const ContinuousColour& c, double scalar) {
    return 1.0/scalar * c;
}

std::ostream& operator<<(std::ostream& os, const ContinuousColour& c) {
    return os << "(" << c.red << ", " << c.green << ", " << c.blue << ")";
}
