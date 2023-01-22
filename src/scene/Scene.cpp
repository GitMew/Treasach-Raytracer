#include "Scene.h"


void Scene::add(SceneObject* o) {
    this->objects.push_back(o);
}


HitInfo Scene::findClosest(const Ray& ray) const {
    double closestT = 1e99;
    SceneObject* closestObject = nullptr;

    for (auto pointer : this->objects) {
        auto newT = pointer->intersect(ray);
        if (newT > 0 && newT < closestT) {
            closestT = newT;
            closestObject = pointer;
        }
    }

    return {closestObject, closestT};
}