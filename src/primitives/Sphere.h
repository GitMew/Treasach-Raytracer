#ifndef TREASACH_SPHERE_H
#define TREASACH_SPHERE_H

#include "SceneObject.h"


class Sphere : public SceneObject {
    public:
        Vector3 center;
        double radius;

        Sphere(const Vector3& center, double radius);

        double intersect(const Ray &ray) const override;
        Vector3 outwardNormal(const Vector3 &p) const override;
        double randomPoint(RNG& rng, Vector3& result) const override;
};


#endif //TREASACH_SPHERE_H
