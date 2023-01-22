#include "Camera.h"


Camera::Camera(const Vector3 &eye, const Vector3 &dir, LensProperties lens)
    : eye(eye), dir(dir), lens(lens) {
    this->viewZ = -1*dir;
    this->viewZ.normalise();

    this->viewX = this->up.cross(this->viewZ);
    this->viewX.normalise();

    this->viewY = this->viewZ.cross(this->viewX);
    this->viewY.normalise();  // Should already be normalised, but just in case for numerical stability.
}


void Camera::generateRays(const Screen& screen, const Pixel& pixel, std::vector<Ray>& rays) const {
    auto newRays = std::vector<Ray>();

    // Center rays  TODO: Anti-aliasing with precomputed jittered grid
    newRays.emplace_back(this->eye,
                            screen.stride*(pixel.x - (screen.width-1)/2.0) *this->viewX
                          + screen.stride*(pixel.y - (screen.height-1)/2.0)*this->viewY
                          - this->lens.viewDistance*this->viewZ,
                          DirectAndIndirect);

    // TODO: For each centre ray, get a focal point

    // TODO: For each focal point: for each precomputed aperture point: make a primary ray from aperture point to aperture point.

    rays = std::move(newRays);
}
