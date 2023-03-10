#include "FlatDisk.h"


FlatDisk::FlatDisk(const Vector3& centre, const Vector3& normal, double radius)
    : centre(centre), n(normal.normalised()), radius(radius) {}

// For explanation, see FlatInfinitePlane::intersect.
double FlatDisk::intersect(const Ray &ray) const {
    // From flat plane
    double denominator = this->n*ray.d;
    if (isBasicallyZero(denominator))
        return -1;
    double t = (this->n*(this->centre - ray.p0))/denominator;

    // New
    Vector3 hitpoint = ray.evaluate(t);
    if ((hitpoint - this->centre).norm() > this->radius)
        return -1;
    return t;
}

Vector3 FlatDisk::outwardNormal(const Vector3 &p) const {
    return this->n;
}

/**
 * Note that the probability density over a disk is a function of r in [0,R] and theta in [0,2pi[:
 *      f(r,theta) = f(r)f(theta)
 * with
 *      f(theta) = 1/2pi
 *      f(r)     = 2*r/R^2
 * and hence
 *      F(theta) = theta/2pi
 *      F(r)     = r^2/R^2
 * and hence
 *      Q(p_theta) = 2pi*p_theta
 *      Q(p_r)     = sqrt(R^2*p_r) = R*sqrt(p_r)
 * https://stackoverflow.com/questions/5837572/generate-a-random-point-within-a-circle-uniformly
 */
double FlatDisk::randomPoint(RNG &rng, Vector3 &result) const {
    // Step 1: generate parameters
    double p1 = rng.rand01();
    double p2 = rng.rand01();

    double theta = 2*M_PI*p1;
    double r     = this->radius*sqrt(p2);

    // Step 2: make coordinate axes; see SphereMath::sampleHemisphere
    Vector3 tangent1 = this->n ^ CROSSPRODUCT_DUMMY;  // x is arbitrary. Assumption: normal and dummy are not parallel.
    tangent1.normalise();
    Vector3 tangent2 = tangent1 ^ this->n;  // z = x ^ y. This is already normalised.

    // Step 3: make coordinates
    double x = r*cos(theta);
    double z = r*sin(theta);

    // Step 4: add to the centre
    result = this->centre + (x*tangent1 + z*tangent2);
    return 2*r/(this->radius*this->radius*2*M_PI);
}
