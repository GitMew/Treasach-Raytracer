#ifndef TREASACH_SCENEOBJECT_H
#define TREASACH_SCENEOBJECT_H


#include "../rendering/Ray.h"

class SceneObject {
    public:
        virtual double intersect(const Ray& ray) = 0;
};


#endif //TREASACH_SCENEOBJECT_H
