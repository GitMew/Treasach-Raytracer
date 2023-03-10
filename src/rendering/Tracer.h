#ifndef TREASACH_TRACER_H
#define TREASACH_TRACER_H

#include "../shading/Colour.h"
#include "../scene/Scene.h"
#include "Camera.h"


class Tracer {

    public:
        virtual ContinuousColour trace(const Scene& scene, const Ray& ray, unsigned recursionsSoFar) = 0;
        virtual RgbColour tonemap(ContinuousColour radiance) = 0;
        void render(const Camera& camera, Screen& screen, const Scene& scene, const std::string& fileOut);

};


#endif //TREASACH_TRACER_H
