#ifndef TREASACH_SPHERE_H
#define TREASACH_SPHERE_H

#include "SceneObject.h"


class Sphere : public SceneObject {
    public:
        Vector center;
        double radius;

        Sphere(const Vector& center, double radius);

        double intersect(const Ray &ray) override;
};


#endif //TREASACH_SPHERE_H
