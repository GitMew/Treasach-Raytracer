#ifndef TREASACH_TRACER_H
#define TREASACH_TRACER_H

#include <string>
#include "Camera.h"
#include "Screen.h"
#include "../scene/Scene.h"
#include "../noise/RNG.h"
#include "../shading/Colour.h"

class Tracer {
    public:
        // Fields (BRDFs have to access these, so they are public)

        RNG rng;

        unsigned shadowIntegralSamples;
        unsigned hemisphericIntegralSamples;
        double rouletteChanceOfDying;
        unsigned minDepth;
        unsigned maxDepth;

        // Methods

        Tracer(unsigned shadowIntegralSamples=50, unsigned hemisphericIntegralSamples=10, double rouletteChanceOfDying=0.1,
               unsigned minDepth=0, unsigned maxDepth=1);
        void render(const Camera& camera, Screen& screen, const Scene& scene, const std::string& fileOut);
        ContinuousColour trace(const Scene& scene, const Ray& ray, unsigned recursionsSoFar);
};


#endif //TREASACH_TRACER_H
