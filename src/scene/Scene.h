#ifndef TREASACH_SCENE_H
#define TREASACH_SCENE_H

#include <vector>
#include "../primitives/SceneObject.h"


class Scene {
    public:
        void add(const SceneObject& o);

    private:
        std::vector<SceneObject> objects = std::vector<SceneObject>();  // TODO: should be stored in a BVH.
};


#endif //TREASACH_SCENE_H
