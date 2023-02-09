#ifndef TREASACH_SCENE_H
#define TREASACH_SCENE_H

#include <vector>
#include "../primitives/SceneObject.h"


struct HitInfo {
    SceneObject* foundObject;
    double t;

    Vector3 hitpoint;       // The point at which we hit.
    Vector3 shadingNormal;  // The shading normal (not the geometric normal) at that point.
    Vector3 omega_o;        // The direction of the ray that caused the hit.
};


class Scene {
    public:
        std::vector<SceneObject*> objects = std::vector<SceneObject*>();  // TODO: should be stored in a BVH.
        std::vector<SceneObject*> lights  = std::vector<SceneObject*>();

        void add(SceneObject* o);
        HitInfo findClosest(const Ray& ray) const;

    private:
};


#endif //TREASACH_SCENE_H
