#ifndef TREASACH_RAY_H
#define TREASACH_RAY_H

#include "../geometry/Vector3.h"

enum RayType {
    DirectAndIndirect,  // Shot through pixels
    Direct,             // Shadow rays
    Indirect            // Hemispheric rays
};

class Ray {
    public:
        Vector3 p0;
        Vector3 d;
        RayType type;

        Ray(const Vector3& start, const Vector3& dir, RayType type);

        Vector3 evaluate(double t) const;
};


#endif //TREASACH_RAY_H
