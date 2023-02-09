#include "Ray.h"

/**
 * Postcondition: the ray direction is normalised.
 * You could wonder if this isn't unnecessary overhead. As it turns out, the ONLY usages of the ray direction are:
 *      1. To be normalised to get omega_o;
 *      2. In intersection math, where, to avoid acne (at a given object scale), you need the same scale of what t
 *         could be for erroneous intersections for all rays. Hence, it's best to normalise the direction.
 * If there were other usages, then doing (1) when needed would be a better idea.
 */
Ray::Ray(const Vector3 &start, const Vector3 &dir, RayType type)
    : p0(start), d(dir.normalised()), type(type) {}


Vector3 Ray::evaluate(double t) const {
    return this->p0 + t*this->d;
}
