#include "Tracer.h"
#include "../shading/BRDF.h"

#include "stb_image_write.h"

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

RgbColour tonemap(ContinuousColour radiance) {
    return {
            tonemapOneChannel(radiance.red),
            tonemapOneChannel(radiance.green),
            tonemapOneChannel(radiance.blue)
    };
}


Tracer::Tracer(unsigned int shadowIntegralSamples, unsigned int hemisphericIntegralSamples,
               double rouletteChanceOfDying, unsigned int minDepth, unsigned int maxDepth)
               : shadowIntegralSamples(shadowIntegralSamples), hemisphericIntegralSamples(hemisphericIntegralSamples),
               rouletteChanceOfDying(rouletteChanceOfDying), minDepth(minDepth), maxDepth(maxDepth) {}


void Tracer::render(const Camera &camera, Screen &screen, const Scene &scene, const std::string& fileOut) {
    // Raytracing
    std::cout << "Raytracing ...\n";
    //#pragma omp parallel
    for (auto& pixel : screen.pixels) {
        auto rays = std::vector<Ray>();
        camera.generateRays(screen, pixel, rays);
        //#pragma omp parallel for reduction (+:pixel.radiance)
        for (auto& ray : rays)
            pixel.radiance += this->trace(scene, ray, 0);  // Accumulate

        pixel.radiance = 1.0/rays.size() * pixel.radiance;  // Average
    }

    // Tone mapping
    std::cout << "Tonemapping ...\n";
    uint8_t pixelArray[3*screen.width*screen.height];
    for (auto& pixel : screen.pixels) {
        RgbColour rgbValue = tonemap(pixel.radiance);
        pixelArray[3*(pixel.y*screen.width + pixel.x) + 0] = rgbValue.red;
        pixelArray[3*(pixel.y*screen.width + pixel.x) + 1] = rgbValue.green;
        pixelArray[3*(pixel.y*screen.width + pixel.x) + 2] = rgbValue.blue;
        //std::cout << pixel << " (" << +rgbValue.red << ", " << +rgbValue.green << ", " << +rgbValue.blue << ")" << "\n";
    }

    // File writing
    std::cout << "Writing file ...\n";
    stbi_write_png(fileOut.c_str(), screen.width, screen.height, 3, pixelArray, 3*screen.width);  // Documentation: https://github.com/nothings/stb/blob/master/stb_image_write.h#L79
}


ContinuousColour Tracer::trace(const Scene& scene, const Ray& ray, unsigned recursionsSoFar) {
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
        bool doRecurse = recursionsSoFar < this->minDepth || (recursionsSoFar < this->maxDepth && rouletteValue > this->rouletteChanceOfDying);

        for (const auto& brdf : hit.foundObject->material->brdfs) {  // If the material has no BRDF, the ray doesn't reproduce by default.
            totalRadiance += brdf->integrateShadow(*this, scene, hit);
            if (doRecurse)  // TODO: I don't think you should roulette in a mirror. Not sure though.
                totalRadiance += 1.0/(1-this->rouletteChanceOfDying) * brdf->integrateHemisphere(*this, scene, hit, recursionsSoFar);  // DON'T increment recursionsSoFar yet, because this call isn't a recursion yet.
        }
    }

    return totalRadiance;
}
