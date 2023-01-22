#include <optional>
#include "Tracer.h"
#include "../shading/BRDF.h"


void Tracer::render(const Camera &camera, Screen &screen, const Scene &scene, std::string fileOut) {
    for (auto& pixel : screen.pixels) {
        auto rays = std::vector<Ray>();
        camera.generateRays(screen, pixel, rays);
        for (auto& ray : rays)
            pixel.c = pixel.c + this->trace(scene, ray);  // Accumulate

        pixel.c = 1/(double)rays.size()*pixel.c;  // Average
    }

    // TODO: Tone mapping from double [0,inf[ to integer [0,255]
    
    // TODO: Write image out to file (probably using OpenCV).
    (void)fileOut;
}


ContinuousColour Tracer::trace(const Scene& scene, const Ray &r) {
    // Find object
    HitInfo hit = scene.findClosest(r);
    if (hit.foundObject == nullptr)  // TODO: Environment map should be at infinity.
        return ContinuousColour({0,0,0});

    Vector3 hitpoint = r.evaluate(hit.t);
    Vector3 normal   = hit.foundObject->normal(hitpoint);

    // Get radiance
    ContinuousColour totalRadiance;

    // - Collect emittance L_e.
    if (r.type != RayType::Indirect) {
        totalRadiance = totalRadiance + hit.foundObject->L_e(hitpoint, r.d);
    }

    // - Collect the integral  TODO: I wonder if you evaluate the direct integral even on a Russian roulette. I think yes.
    if (r.type != RayType::Direct) {
        // BRDF = 0? Then stop (probably a light source, or a black hole).

        // Russian roulette for whether we stop.

        // Else: it's rendering equation time!
        //  - Shitty implementation: one hemispheric integral.
        //  - Good implementation:
        //      - Direct illumination with an integral over all light surfaces with type=Direct.
        //      - Indirect illumination with a hemispheric integral with type=Indirect.

        for (BRDF brdf : hit.foundObject.brdfs) {
            auto directions = std::vector<Vector3>();
            brdf.generateDirections(directions);

            ContinuousColour newRadiance;
            for (dir : directions) {
                auto brdfValue = brdf.evaluate(dir);
                auto recursiveRadiance = this->trace(scene, Ray(hitpoint, dir, Indirect));
                newRadiance = newRadiance + brdfValue*cos(dir)*recursiveRadiance;
            }
            newRadiance = newRadiance/directions.size();

            totalRadiance = totalRadiance + newRadiance;
        }
    }

    return totalRadiance;
}