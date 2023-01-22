#include "Ray.h"

Ray::Ray(const Vector3 &start, const Vector3 &dir, RayType type)
    : p0(start), d(dir), type(type) {}


Vector3 Ray::evaluate(double t) const {
    return this->p0 + t*this->d;
}
