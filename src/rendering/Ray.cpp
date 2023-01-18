#include "Ray.h"

Ray::Ray(const Vector &start, const Vector &dir, RayType type)
    : p0(start), d(dir), type(type) {}


Vector Ray::evaluate(double t) const {
    return this->p0 + t*this->d;
}
