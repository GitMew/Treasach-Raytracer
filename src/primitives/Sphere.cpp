#include "Sphere.h"

#include "../geometry/SphereMath.h"

Sphere::Sphere(const Vector3 &center, double radius)
    : center(center), radius(radius) {}


double Sphere::intersect(const Ray &ray) const {
    double a = ray.d * ray.d;
    double b = 2*ray.d*(ray.p0 - this->center);
    double c = (ray.p0 - this->center) * (ray.p0 - this->center) - this->radius*this->radius;
    double d = b*b - 4*a*c;
    if (d < 0)
        return -1;

    double sd = sqrt(d);
    double negativeSolution = -b - sd;
    double positiveSolution = -b + sd;

    // To prevent a branch, a piecewise expression is used here.
    // There can be 2 solutions or 1 solution, but the latter is a special case of the former.
    // The 2 solutions can either both be invalid (<0), only the positive can be valid, or both can be valid.
    //      - When the negative is invalid, return the positive.
    //      - When both are invalid (so also the negative), returning either is fine (so also the positive).
    //      - When the negative is valid, return it.
    // Also, because we are comparing t with 0, we must use the acne threshold, not 0.
    bool ignoreNegative = isNegativeOrAcne(negativeSolution);

    return (negativeSolution*(!ignoreNegative) + positiveSolution*ignoreNegative) / (2*a);
}


Vector3 Sphere::outwardNormal(const Vector3 &p) const {
    auto normal = p - this->center;
    return normal.normalised();
}

double Sphere::randomPoint(RNG& rng, Vector3& result) const {
    double theta = 2*M_PI*rng.rand01();  // 1/2pi
    double phi   = M_PI*rng.rand01();    // 1/pi

    result = this->center + sphericalToCartesian(theta, phi, this->radius);  // Note that reassigning a reference ripples up to the caller.
    return M_1_PI*M_1_PI/2.0;
}
