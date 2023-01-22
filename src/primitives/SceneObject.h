#ifndef TREASACH_SCENEOBJECT_H
#define TREASACH_SCENEOBJECT_H


#include "../rendering/Ray.h"

class SceneObject {
    public:
        virtual double intersect(const Ray& ray) const = 0;
        // Assumes p is on the object.
        virtual Vector3 normal(const Vector3& p) const = 0;
        //virtual ContinuousColour L_e(const Vector3& p, const Vector3& omega_o) const = 0;  // Shouldn't be part of the object geometry, but of the shading, like the BRDF.
};


#endif //TREASACH_SCENEOBJECT_H
