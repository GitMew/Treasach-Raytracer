#ifndef TREASACH_SCENE_H
#define TREASACH_SCENE_H

#include <vector>
#include "../primitives/SceneObject.h"

struct HitInfo {
    SceneObject* foundObject;
    double t;
};


class Scene {
    public:
        std::vector<SceneObject*> objects = std::vector<SceneObject*>();  // TODO: should be stored in a BVH.

        void add(SceneObject* o);
        HitInfo findClosest(const Ray& ray) const;

    private:
};


#endif //TREASACH_SCENE_H
