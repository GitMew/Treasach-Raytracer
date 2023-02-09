#include "EmissionFunction.h"
#include "../primitives/SceneObject.h"


NoEmission::NoEmission() {}

ContinuousColour NoEmission::radiance(const SceneObject &surface, const Vector3 &p, const Vector3 &omega_o) {
    return {0,0,0};
}

UniformEmission::UniformEmission(const ContinuousColour &value)
    : value(value) {}

ContinuousColour UniformEmission::radiance(const SceneObject &surface, const Vector3 &p, const Vector3 &omega_o) {
    return this->value;
}


SpotlightEmission::SpotlightEmission(const ContinuousColour &value, double minimalCosine)
    : value(value), minimalCosine(minimalCosine) {}

/**
 * Precondition: omega_o must be normalised.
 */
ContinuousColour SpotlightEmission::radiance(const SceneObject &surface, const Vector3 &p, const Vector3 &omega_o) {
    if (surface.normal(p, omega_o) * omega_o < this->minimalCosine) {
        return {0,0,0};
    } else {
        return this->value;
    }
}

