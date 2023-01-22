#ifndef TREASACH_TRACER_H
#define TREASACH_TRACER_H

#include <string>
#include "Camera.h"
#include "Screen.h"
#include "../scene/Scene.h"


class Tracer {
    public:
        void render(const Camera& camera, Screen& screen, const Scene& scene, std::string fileOut);
        ContinuousColour trace(const Scene& scene, const Ray& r);
};


#endif //TREASACH_TRACER_H
