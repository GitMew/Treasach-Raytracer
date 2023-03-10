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
        if (!isNegativeOrAcne(newT) && newT < closestT) {  // Note: the fact that this function tests for acne does NOT mean SceneObject::intersect doesn't have to. For example, a sphere must check that its first solution isn't below the acne threshold so it knows to return its second solution.
            closestT = newT;
            closestObject = pointer;
        }
    }

    return {closestObject, closestT};
}