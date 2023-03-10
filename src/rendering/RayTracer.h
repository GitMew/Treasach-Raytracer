#ifndef TREASACH_RAYTRACER_H
#define TREASACH_RAYTRACER_H

#include <string>

#include "Camera.h"
#include "Screen.h"
#include "Tracer.h"
#include "../scene/Scene.h"
#include "../noise/RNG.h"
#include "../shading/Colour.h"


class RayTracer : public Tracer {
    public:
        // Fields (BRDFs have to access these, so they are public)

        RNG rng;

        unsigned shadowIntegralSamples;
        unsigned hemisphericIntegralSamples;
        double rouletteChanceOfDying;
        unsigned minDepth;
        unsigned maxDepth;

        // Methods

        RayTracer(unsigned shadowIntegralSamples=50, unsigned hemisphericIntegralSamples=10, double rouletteChanceOfDying=0.1,
                  unsigned minDepth=0, unsigned maxDepth=1);

        bool stoppingCriterion(unsigned recursionsSoFar, double rouletteValue);

        ContinuousColour trace(const Scene& scene, const Ray& ray, unsigned recursionsSoFar);
        RgbColour tonemap(ContinuousColour radiance);
};


#endif //TREASACH_RAYTRACER_H
