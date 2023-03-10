#include "NormalTracer.h"

ContinuousColour NormalTracer::trace(const Scene &scene, const Ray &ray, unsigned int recursionsSoFar) {
    // 1. Find object
    HitInfo hit = scene.findClosest(ray);
    if (hit.foundObject == nullptr)
        return ContinuousColour({0,0,0});

    hit.hitpoint = ray.evaluate(hit.t);
    hit.omega_o  = -ray.d.normalised();  // Since I am not sure about whether I'll keep ray normalisation, I have this here.
    hit.shadingNormal = hit.foundObject->normal(hit.hitpoint, hit.omega_o);

    // 2. Turn normal into colour. Normals should be normalised, so the resulting channels are in [-1,1].
    return {
        hit.shadingNormal.x,
        hit.shadingNormal.y,
        hit.shadingNormal.z
    };
}


RgbColour NormalTracer::tonemap(ContinuousColour radiance) {
    return {
        static_cast<uint8_t>(255*(1+radiance.red)/2),
        static_cast<uint8_t>(255*(1+radiance.green)/2),
        static_cast<uint8_t>(255*(1+radiance.blue)/2)
    };
}

