#include "RayTracer.h"
#include "../shading/BRDF.h"

#include <iostream>
//#include <glm.hpp> // glm::vec3

/**
 * floor(256*x/(x+1)) maps x in [0,+infty[ to the integer interval [0,255].
 */
uint8_t tonemapOneChannel(double radiance) {
    //if (radiance < 0)
    //    std::cout << "WARNING: Negative radiance detected (" << radiance << "). Will cause rollover to extreme brightness!\n";
    return static_cast<uint8_t>(256*radiance/(radiance+1));
}

RgbColour RayTracer::tonemap(ContinuousColour radiance) {
    return {
            tonemapOneChannel(radiance.red),
            tonemapOneChannel(radiance.green),
            tonemapOneChannel(radiance.blue)
    };
}


RayTracer::RayTracer(unsigned int shadowIntegralSamples, unsigned int hemisphericIntegralSamples,
                     double rouletteChanceOfDying, unsigned int minDepth, unsigned int maxDepth)
               : shadowIntegralSamples(shadowIntegralSamples), hemisphericIntegralSamples(hemisphericIntegralSamples),
               rouletteChanceOfDying(rouletteChanceOfDying), minDepth(minDepth), maxDepth(maxDepth) {}


ContinuousColour RayTracer::trace(const Scene& scene, const Ray& ray, unsigned recursionsSoFar) {
    // 1. Find object
    HitInfo hit = scene.findClosest(ray);
    if (hit.foundObject == nullptr)
        return ContinuousColour({0.02,0.02,0.02});  // TODO: Environment map should be at infinity.
    hit.hitpoint = ray.evaluate(hit.t);
    hit.omega_o  = -ray.d.normalised();  // Since I am not sure about whether I'll keep ray normalisation, I have this here.

    // 2. Get radiance
    ContinuousColour totalRadiance;

    // 2a. Collect intrinsic radiance L_e.
    if (ray.type != RayType::OnlyIntegrals) {
        if (hit.foundObject->material->emitter != nullptr) {
            totalRadiance += hit.foundObject->material->emitter->radiance(*hit.foundObject, hit.hitpoint, hit.omega_o);
            return totalRadiance;  // NOTE: In theory, you shouldn't stop here, but it is a neat way of cutting recursion.
        }
    }

    // 2b. Collect extrinsic radiance, i.e. the two integrals.
    if (ray.type != RayType::OnlyEmission) {
        hit.shadingNormal = hit.foundObject->normal(hit.hitpoint, hit.omega_o);

        double rouletteValue = rng.rand01();  // Russian roulette for whether the recursive integral is evaluated. The non-recursive one ALWAYS happens (so that not ending in a light still has value).

        for (const auto& brdf : hit.foundObject->material->brdfs) {  // If the material has no BRDF, the ray doesn't reproduce by default.
            totalRadiance += brdf->integrateShadow(*this, scene, hit);
            totalRadiance += brdf->integrateHemisphere(*this, scene, hit, recursionsSoFar, rouletteValue);  // DON'T increment recursionsSoFar yet, because this call isn't a recursion yet.
        }
    }

    return totalRadiance;
}


/**
 * Takes care of minimal recursion depth (never stop), maximal recursion depth (always stop), and
 * Russian roulette (stop if it falls below the threshold).
 * If you don't want Roulette, set rouletteValue to 1.0 (equivalent to chance of dying 0).
 * If you want to kill rays after 1 recursion, use 0.0 (equivalent to chance of dying 1).
 */
// TODO: Quadruple check that this is correct, because I don't think it is.
bool RayTracer::stoppingCriterion(unsigned recursionsSoFar, double rouletteValue) {
    return recursionsSoFar >= this->minDepth &&
          (recursionsSoFar > this->maxDepth || rouletteValue < this->rouletteChanceOfDying);
}