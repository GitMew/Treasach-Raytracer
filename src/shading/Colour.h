#ifndef TREASACH_COLOUR_H
#define TREASACH_COLOUR_H

#include <cstdint>
#include <ostream>

class ContinuousColour {
    public:
        double red   = 0;
        double green = 0;
        double blue  = 0;

        ContinuousColour operator+(const ContinuousColour& other) const;
        void operator+=(const ContinuousColour& other);
};

ContinuousColour operator*(double scalar, const ContinuousColour& c);
std::ostream& operator<<(std::ostream& os, const ContinuousColour& c);


struct RgbColour {
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
};


#endif //TREASACH_COLOUR_H
