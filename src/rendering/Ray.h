#ifndef TREASACH_RAY_H
#define TREASACH_RAY_H

#include "../geometry/Vector3.h"

enum RayType {
    All,           // Shot through pixels
    OnlyEmission,  // Only retrieves L_e
    OnlyIntegrals  // Retrieves the two integral terms (area integral of intrinsic + hemispheric integral of extrinsic)
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
