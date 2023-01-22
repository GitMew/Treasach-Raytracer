#include "Sphere.h"


Sphere::Sphere(const Vector3 &center, double radius)
    : center(center), radius(radius) {}


double Sphere::intersect(const Ray &ray) const {  // FIXME: Implement this
    (void)ray;
    return 0;
}


Vector3 Sphere::normal(const Vector3 &p) const {
    auto normal = p - this->center;
    normal.normalise();
    return normal;
}
