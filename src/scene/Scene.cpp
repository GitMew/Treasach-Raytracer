#include "Scene.h"

void Scene::add(SceneObject* o) {
    this->objects.push_back(o);
    if (o->material->emitter != nullptr)
        this->lights.push_back(o);
}


HitInfo Scene::findClosest(const Ray& ray) const {
    double closestT = 1e99;
    SceneObject* closestObject = nullptr;

    for (auto pointer : this->objects) {
        auto newT = pointer->intersect(ray);
        if (INTERSECTION_TOLERANCE < newT && newT < closestT) {
            closestT = newT;
            closestObject = pointer;
        }
    }

    return {closestObject, closestT};
}