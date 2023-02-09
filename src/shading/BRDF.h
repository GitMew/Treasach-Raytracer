#ifndef TREASACH_BRDF_H
#define TREASACH_BRDF_H

#include "../rendering/Tracer.h"
#include "../scene/Scene.h"


class BRDF {
    public:
        virtual ContinuousColour integrateShadow    (Tracer& tracer, const Scene& scene, const HitInfo& hit) = 0;
        virtual ContinuousColour integrateHemisphere(Tracer& tracer, const Scene& scene, const HitInfo& hit, unsigned recursionsSoFar) = 0;

        //virtual void generateDirections(const Vector3& p, const Vector3& normal, const Vector3& outgoingRay, std::vector<Vector3>& rays) = 0;
        //virtual double evaluate(const Vector3& p, const Vector3& normal, const Vector3& incomingRay, const Vector3& outgoingRay) = 0;
};


class DiffuseBRDF : public BRDF {
    public:
        double k_d;
        DiffuseBRDF(double diffuseCoefficient);

        ContinuousColour integrateShadow    (Tracer& tracer, const Scene& scene, const HitInfo& hit) override;
        ContinuousColour integrateHemisphere(Tracer& tracer, const Scene& scene, const HitInfo& hit, unsigned recursionsSoFar) override;
};


/**
 * Limit of the Fresnel equations where R(phi) = 1 for all phi. This is possible when the refractive index of the
 * material (n_2 in the Fresnel equations) is infinite.
 */
class MirrorBRDF : public BRDF {
    public:
        ContinuousColour integrateShadow    (Tracer& tracer, const Scene& scene, const HitInfo& hit) override;
        ContinuousColour integrateHemisphere(Tracer& tracer, const Scene& scene, const HitInfo& hit, unsigned recursionsSoFar) override;
};

/**
 * There are no "transmissive mirrors", i.e. R(phi) = 0 and T(phi) = 1. In FresnelBRDF, I discuss why R(phi) = 0 can't
 * be modelled by an absorption term, and the only other way that R(phi) can be 0 is by solving its equation for an n_2
 * that makes it so independent of phi. The only n_2 that does this is n_1; in other words, a "transmissive mirror" is
 * an interface between the material and itself.
 */
//class TransmissionBRDF : public BRDF {
//    public:
//        double refractiveIndex;
//        TransmissionBRDF(double refractiveIndex);
//
//        ContinuousColour integrateShadow    (Tracer& tracer, const Scene& scene, const HitInfo& hit) override;
//        ContinuousColour integrateHemisphere(Tracer& tracer, const Scene& scene, const HitInfo& hit, unsigned recursionsSoFar) override;
//};


/**
 * Weighs reflection and transmission by the Fresnel coefficients R(phi) respectively T(phi).
 *
 * Conservation of energy says R(phi) + T(phi) = 1. When a material is opaque, what happens is that the transmitted
 * rays are attenuated so quickly that they become worthless. For such materials, it is pointless to refract and apply
 * Beer's law, because you know it will result in 0 very quickly. This can also be modelled as conservation of energy
 * having three terms: R(phi) + T(phi) + A(phi) = 1, with A(phi) = 1 - R(phi) the absorption, which implies T(phi) = 0.
 * This is perfectly plausible for e.g. a hard, polished floor, which looks like it has no refraction but still has a
 * varying reflectance with viewing angle.
 *
 * The converse, i.e. pretending R(phi) = 0 but T(phi) still varying with phi instead of being a constant 1, so
 * A(phi) = 1 - T(phi), isn't possible, since this "pretending" would be based in saying that "it is pointless to reflect
 * and apply Beer's law because you know it will result in 0 very quickly". In other words, the current ray is already
 * inside an opaque material, which the above simplification already said would make the ray 0 in the first place.
 */
class FresnelBRDF : public BRDF {
    public:
        bool opaque;
        double refractiveIndex;  // TODO: Should be stored in the material, not in a BRDF.
        FresnelBRDF(bool opaque, double refractiveIndex);

        ContinuousColour integrateShadow    (Tracer& tracer, const Scene& scene, const HitInfo& hit) override;
        ContinuousColour integrateHemisphere(Tracer& tracer, const Scene& scene, const HitInfo& hit, unsigned recursionsSoFar) override;
};

//class GlossyBRDF : public BRDF {
//    public:
//        double exponent;
//        GlossyBRDF(double exponent);
//
//        ContinuousColour integrateShadow    (Tracer& tracer, const Scene& scene, const HitInfo& hit) override;
//        ContinuousColour integrateHemisphere(Tracer& tracer, const Scene& scene, const HitInfo& hit, unsigned recursionsSoFar) override;
//};



#endif //TREASACH_BRDF_H
