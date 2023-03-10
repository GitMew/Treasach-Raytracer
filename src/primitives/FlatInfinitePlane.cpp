#include "FlatInfinitePlane.h"
#include "../geometry/Constants.h"

FlatInfinitePlane::FlatInfinitePlane(const Vector3& centre, const Vector3& normal)
    : centre(centre), n(normal.normalised()) {}

// Derived from the plane equation
//      normal * (p(t) - c) = 0
// with p(t) = p0 + t d,
//      normal * (p0 - c) + (normal * d) t = 0
double FlatInfinitePlane::intersect(const Ray &ray) const {
    double denominator = this->n*ray.d;
    if (isBasicallyZero(denominator))
        return -1;
    return (this->n*(this->centre - ray.p0))/denominator;
}

Vector3 FlatInfinitePlane::outwardNormal(const Vector3 &p) const {
    return this->n;
}

// Should not be used. The plane is infinitely large.
double FlatInfinitePlane::randomPoint(RNG &rng, Vector3 &result) const {
    result = this->centre;
    return 0;
}
