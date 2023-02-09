#ifndef TREASACH_FLATDISK_H
#define TREASACH_FLATDISK_H

#include "SceneObject.h"


/**
 * I'm too lazy to figure out the intersection rule for finite flat rectangles, but for disks it's easy.
 */
class FlatDisk : public SceneObject {
    public:
        FlatDisk(const Vector3& centre, const Vector3& normal, double radius);

        double intersect(const Ray &ray) const override;
        Vector3 outwardNormal(const Vector3 &p) const override;
        double randomPoint(RNG &rng, Vector3 &result) const override;

    private:
        Vector3 centre;
        Vector3 n;
        double radius;
};


#endif //TREASACH_FLATDISK_H
