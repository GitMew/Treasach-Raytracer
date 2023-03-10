#ifndef TREASACH_AABB_H
#define TREASACH_AABB_H


#include "SceneObject.h"

class AABB : public SceneObject {
    public:
        AABB(const Vector3& minimalCorner, const Vector3& maximalCorner);

        double intersect(const Ray &ray) const override;
        Vector3 outwardNormal(const Vector3 &p) const override;
        double randomPoint(RNG &rng, Vector3 &result) const override;

    private:
        Vector3 minimalCorner;
        Vector3 maximalCorner;
};


#endif //TREASACH_AABB_H
