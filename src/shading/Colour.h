#ifndef TREASACH_COLOUR_H
#define TREASACH_COLOUR_H

#include <cstdint>
#include <ostream>

class ContinuousColour {
    public:
        double red   = 0;
        double green = 0;
        double blue  = 0;

        ContinuousColour();
        ContinuousColour(double red, double green, double blue);

        ContinuousColour operator+(const ContinuousColour& other) const;
        void operator+=(const ContinuousColour& other);
        ContinuousColour L1normalised() const;
};

ContinuousColour operator*(const ContinuousColour& c1, const ContinuousColour& c2);
ContinuousColour operator*(double scalar, const ContinuousColour& c);
ContinuousColour operator*(const ContinuousColour& c, double scalar);
ContinuousColour operator/(const ContinuousColour& c, double scalar);
std::ostream& operator<<(std::ostream& os, const ContinuousColour& c);

struct RgbColour {
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
};


#endif //TREASACH_COLOUR_H
