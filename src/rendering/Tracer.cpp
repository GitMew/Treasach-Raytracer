#include <optional>
#include "Tracer.h"


void Tracer::render(const Camera &camera, const Screen &screen, const Scene &scene, std::string fileOut) {
    auto pixels = screen.generatePixels();
    for (auto& pixel : pixels) {
        auto rays = camera.generateRays(pixel);  // TODO: Use raytype DirectAndIndirect
        for (auto& ray : rays) {
            auto colour = this->trace(scene, ray);
            // TODO: Find a good way to store them before averaging into the pixel.
        }
        // TODO: Average into the pixel AND do tone mapping.
    }

    // TODO: Write image out to file (probably using OpenCV).
}


Colour Tracer::trace(const Scene& scene, const Ray &r) {
    // TODO: Find nearest hitpoint
    double closestT = 1e99;
    std::optional<SceneObject&> closestObject;
    for (auto& object : scene.objects) {
        auto newT = object.intersect(r);
        if (newT > 0 && newT < closestT) {
            closestT = newT;
            closestObject = object;
        }
    }
    if (!closestObject.has_value())  // TODO: Environment map should be at infinity.
        return Colour({0,0,0});

    Vector hitpoint = r.evaluate(closestT);
    Vector normal   = closestObject.normal(hitpoint);

    // TODO: Get radiance
    Colour totalRadiance;

    // Collect emittance L_e.
    if (r.type != RayType::Indirect) {

    }

    // Collect the integral
    if (r.type != RayType::Direct) {
        // BRDF = 0? Then stop (probably a light source, or a black hole).

        // Russian roulette for whether we stop.

        // Else: it's rendering equation time!
        //  - Shitty implementation: one hemispheric integral.
        //  - Good implementation:
        //      - Direct illumination with an integral over all light surfaces with type=Direct.
        //      - Indirect illumination with a hemispheric integral with type=Indirect.
    }

    return totalRadiance;
}