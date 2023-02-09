#include "Camera.h"
#include "../noise/RNG.h"
#include "../primitives/FlatInfinitePlane.h"


Camera::Camera(const Vector3 &eye, const Vector3 &dir, LensProperties lens)
    : eye(eye), dir(dir), lens(lens) {
    // View coordinates
    this->viewZ = -this->dir;
    this->viewZ.normalise();

    this->viewX = this->up ^ this->viewZ;
    this->viewX.normalise();

    this->viewY = this->viewZ ^ this->viewX;
    this->viewY.normalise();  // Should already be normalised, but just in case for numerical stability.

    // Anti-aliasing offsets with jitter
    double step = 1.0/lens.antiAliasingDensity;
    RNG rng;
    for (int i = 0; i < lens.antiAliasingDensity; i++) {
        for (int j = 0; j < lens.antiAliasingDensity; j++) {
            // Square grid
            double x = (i + 0.5)*step;
            double y = (j + 0.5)*step;

            // Jitter (note: jittered points can never get into each other's jitter range)
            x += step*(rng.rand01() - 0.5);
            y += step*(rng.rand01() - 0.5);

            this->intraPixelGrid.emplace_back(x, y);
        }
    }

    // Aperture points (uniform circular distribution)
    for (int i = 0; i < lens.focalPointAmount; i++) {
        double theta  = 2*M_PI*rng.rand01();
        double radius = this->lens.aperture*sqrt(rng.rand01());

        this->apertureDisk.emplace_back(radius*cos(theta), radius*sin(theta));
    }
}


void Camera::generateRays(const Screen& screen, const Pixel& pixel, std::vector<Ray>& rays) const {
    // Center rays
    auto centreRays = std::vector<Ray>();
    for (auto pair : this->intraPixelGrid) {
        centreRays.emplace_back(
            this->eye,
            screen.stride   * (pixel.x - screen.width /2.0 + pair.first)  * this->viewX
            + screen.stride * (screen.height/2.0 - pixel.y + pair.second) * this->viewY  // Note that the pixel grid's top left is (0,0) whilst this is the bottom left in view coordinates.
            - this->lens.viewDistance * this->viewZ,
            All
        );
    }

    // Find focal points
    FlatInfinitePlane focalPlane = FlatInfinitePlane(-this->lens.focalDistance*this->viewZ, this->viewZ);
    auto primaryRays = std::vector<Ray>();
    for (auto centreRay : centreRays) {
        auto t = focalPlane.intersect(centreRay);
        auto focalPoint = centreRay.evaluate(t);

        // For each point in the aperture: shoot primary ray to the focal point.
        for (auto apertureOffset : this->apertureDisk) {
            auto aperturePoint = apertureOffset.first*this->viewX + apertureOffset.second*this->viewY;
            primaryRays.emplace_back(
                aperturePoint,
                focalPoint - aperturePoint,
                All
            );
        }
    }

    // Concatenate https://stackoverflow.com/a/21972296/9352077
    centreRays.insert(centreRays.end(),
                      std::make_move_iterator(primaryRays.begin()),
                      std::make_move_iterator(primaryRays.end()));
    rays = std::move(centreRays);
}
