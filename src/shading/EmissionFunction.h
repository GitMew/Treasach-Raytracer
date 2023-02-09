#ifndef TREASACH_EMISSIONFUNCTION_H
#define TREASACH_EMISSIONFUNCTION_H


#include "Colour.h"
#include "../geometry/Vector3.h"
class SceneObject;


/**
 * Model for L_e(p, omega). Just like the hemispheric integral can ask the Tracer for L_i, the shadow integral can ask
 * a light for L_e.
 */
class EmissionFunction {
    public:
        virtual ContinuousColour radiance(const SceneObject &surface, const Vector3 &p, const Vector3 &omega_o) = 0;
};

class NoEmission : public EmissionFunction {
    public:
        NoEmission();
        ContinuousColour radiance(const SceneObject &surface, const Vector3 &p, const Vector3 &omega_o) override;
};

class UniformEmission : public EmissionFunction {
    public:
        UniformEmission(const ContinuousColour& value);
        ContinuousColour radiance(const SceneObject &surface, const Vector3 &p, const Vector3 &omega_o) override;

    private:
        ContinuousColour value;
};

class SpotlightEmission : public EmissionFunction {
    public:
        SpotlightEmission(const ContinuousColour& value, double minimalCosine);
        ContinuousColour radiance(const SceneObject &surface, const Vector3 &p, const Vector3 &omega_o) override;

    private:
        ContinuousColour value;
        double minimalCosine;
};

#endif //TREASACH_EMISSIONFUNCTION_H
