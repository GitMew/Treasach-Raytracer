#include "Scene.h"

void Scene::add(const SceneObject &o) {
    this->objects.push_back(o);  // FIXME: This errors because there is no copy constructor defined for scene objects, apparently. https://www.reddit.com/r/cpp_questions/comments/ukld3u/no_matching_function_for_call_to_construct_at/
}