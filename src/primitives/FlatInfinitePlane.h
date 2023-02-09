#ifndef TREASACH_FLATINFINITEPLANE_H
#define TREASACH_FLATINFINITEPLANE_H

#include "SceneObject.h"


class FlatInfinitePlane : public SceneObject {
    public:
        FlatInfinitePlane(const Vector3& centre, const Vector3& normal);

        double intersect(const Ray &ray) const override;
        Vector3 outwardNormal(const Vector3 &p) const override;
        double randomPoint(RNG &rng, Vector3 &result) const override;

    private:
        Vector3 centre;
        Vector3 n;
};


#endif //TREASACH_FLATINFINITEPLANE_H
