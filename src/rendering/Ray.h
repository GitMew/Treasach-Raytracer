#ifndef TREASACH_RAY_H
#define TREASACH_RAY_H

#include "../geometry/Vector.h"

enum RayType {
    DirectAndIndirect,  // Shot through pixels
    Direct,             // Shadow rays
    Indirect            // Hemispheric rays
};

class Ray {
    public:
        Vector p0;
        Vector d;
        RayType type;

        Ray(const Vector& start, const Vector& dir, RayType type);

        Vector evaluate(double t) const;
};


#endif //TREASACH_RAY_H
