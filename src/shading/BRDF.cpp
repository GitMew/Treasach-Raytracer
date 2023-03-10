#include "BRDF.h"
#include "../rendering/Screen.h"
#include "../geometry/SphereMath.h"
#include <iostream>


/**
 * Diffuse
 */
DiffuseBRDF::DiffuseBRDF(double diffuseCoefficient, RgbColour colour)
    : k_d(diffuseCoefficient*ContinuousColour((double)colour.red/255, (double)colour.green/255, (double)colour.blue/255)) {}

DiffuseBRDF::DiffuseBRDF(double diffuseCoefficient)
    : DiffuseBRDF(diffuseCoefficient, {255,255,255}) {}

ContinuousColour DiffuseBRDF::integrateShadow(RayTracer &tracer, const Scene &scene, const HitInfo &hit) {
    ContinuousColour totalRadiance;

    // If there are no lights, you can't sample any!
    size_t L = scene.lights.size();
    if (L == 0)
        return totalRadiance;

    // Recursive raytrace
    //TODO: according to slide 17 at https://courses.grainger.illinois.edu/cs484/sp2020/6_merged.pdf,
    //  the following pragma should parallelise the loop, yet it doesn't seem to do much ...
    //#pragma omp parallel for reduction (+:totalRadiance)
    for (int rayIndex = 0; rayIndex < tracer.shadowIntegralSamples; rayIndex++) {
        // Step 1a: Sample light
        SceneObject* light = scene.lights[tracer.rng.randInt(L)];
        double discreteProbability = 1.0/L;

        // Step 1b: Generate random point p' on that light
        Vector3 randomPoint;
        double conditionalProbability = light->randomPoint(tracer.rng, randomPoint);

        // Step 2: Shoot shadowRay to p'
        Vector3 interline = randomPoint - hit.hitpoint;
        Ray shadowRay = Ray(hit.hitpoint, interline, OnlyEmission);
        if (shadowRay.d * hit.shadingNormal < 0)  // Prematurely stop if it goes through the surface.
            continue;                             // For closed shapes, this is unnecessary, since the opposing side will block the ray in the visibility check.

        // Step 3: Visibility check
        // What follows between "---" is a boiled-down version of tracer.trace(shadowRay). The reason we can't use it is
        // that we need to verify that we hit the light we sampled above, which the tracer doesn't check nor allow us to check.
        // ---
        //   TODO: doesn't support point lights; for those, you should merely check whether there is any hit at all BEFORE the point light.
        //         This is also a more efficient way of handling other lights: you already know the intersection point p' and how to
        //         calculate the normal for it, so you just need to check if there is ANY hit (not just the closest) between p and p' and can return early if so.
        HitInfo newHit = scene.findClosest(shadowRay);
        if (newHit.foundObject != light) {
            //std::cout << "Visibility check says no.\n";
            continue;
        }

        // Step 4: Get the emitted light L_e
        newHit.omega_o                   = -shadowRay.d.normalised();
        newHit.shadingNormal             = light->normal(randomPoint, newHit.omega_o);
        ContinuousColour emittedRadiance = light->material->emitter->radiance(*light, randomPoint, newHit.omega_o);
        // ---

        // Step 5: The integrand has no Jacobian, and is:   TODO: Actually, does a curved light surface not have a Jacobian when doing the dA integral? Hmmm ...
        //      BRDF*L_e*V*G = BRDF*L_e*cos(phi)*cos(phi')/||p' - p||^2
        //                   = BRDF*L_e*(n dot omega_i)*(n' dot -omega_i)/||p' - p||^2
        // where V = 1 inside this `if`. Since the PDF here is P(light)*P(p' | light), you can't divide out any of
        // these factors, and have to just divide by the PDF instead.
        // In this, omega_i is the direction from p to p', but you can also use the shadowRay from p' to p.
        ContinuousColour restOfSummand = -M_1_PI * this->k_d * (hit.shadingNormal*newHit.omega_o)*(newHit.shadingNormal*newHit.omega_o)/((interline * interline) * discreteProbability*conditionalProbability);
        totalRadiance += restOfSummand*emittedRadiance;
    }
    return 1.0/tracer.shadowIntegralSamples * totalRadiance;
}


ContinuousColour DiffuseBRDF::integrateHemisphere(RayTracer &tracer, const Scene &scene, const HitInfo &hit, unsigned recursionsSoFar, double rouletteValue) {
    if (tracer.stoppingCriterion(recursionsSoFar, rouletteValue))
        return {0,0,0};

    ContinuousColour totalRadiance;
    for (int i = 0; i < tracer.hemisphericIntegralSamples; i++) {
        // Step 1: Generate random point
        Vector3 randomDirection;
        sampleHemisphereBiased(tracer.rng, hit.hitpoint, randomDirection);  // The return value is unused because the PDF cancels in the integrand.

        // Step 2: Trace
        Ray ray = Ray(hit.hitpoint, randomDirection, OnlyIntegrals);
        ContinuousColour incomingRadiance = tracer.trace(scene, ray, recursionsSoFar+1);

        // Step 3: The integrand with Jacobian is
        //      BRDF*incomingRadiance*cos(phi)*sin(phi)
        // The Monte Carlo summand is
        //      integrand/pdf = integrand/(1/2pi * 2sin(phi)*cos(phi))
        //                    = pi*BRDF*incomingRadiance
        //                    = k_d*incomingRadiance
        totalRadiance += this->k_d*incomingRadiance;
        std::cout << "Recursion level: " << recursionsSoFar << "\n";
        std::cout << "Incoming: " << incomingRadiance << "\n";
        std::cout << "Total: " << totalRadiance << "\n";
    }
    return 1.0/(1-tracer.rouletteChanceOfDying) * (1.0/tracer.hemisphericIntegralSamples * totalRadiance);
}

/**
 * Perfect mirror
 */
/**
 * The perfect mirror has no shadow integral, because its BRDF is a hemispheric delta function.
 *
 * Like for caustics, this is an strategy that potentially hurts a backwards raytracer. If all the
 * light in the scene can only be seen via a mirror, clearly the mirror should be treated as if it
 * was a light source itself.
 */
ContinuousColour MirrorBRDF::integrateShadow(RayTracer &tracer, const Scene &scene, const HitInfo &hit) {
    return {0,0,0};
}

/**
 * A couple of notes on this implementation, even if it is short:
 *  - Why ray type "All"? Because a mirror BRDF is a delta function of the UNSPLIT rendering integral,
 *    which is 1. hemispheric (hence there is no shadow integral) and 2. contains the FULL incoming radiance,
 *    both integrated AND L_e. This differs from the diffuse hemispheric integral, which can't see L_e.
 *    An intuitive explanation is that you otherwise couldn't see lights in a mirror.
 *  - Why no Russian roulette kill? Because stopping on a mirror causes pepper noise.
 *  - Why the incrementRecursionDepth rule? It comes down to three things:
 *      1. Mirrors/refractions are actually way cheaper than a diffuse/Phong integral because they have only 1 ray
 *         (the "ray tree" doesn't widen), so we should allow them to recurse deeper than the integrals get to recurse.
 *      2. If you count the mirror bounce as a recursion deeper, the mirror image is less shaded than the actual object
 *         since you land on the object with e.g. no depth left to do an indirect lighting integral.
 *      3. You nevertheless need at least some mechanism to prevent infinite bounces between mirrors.
 */
ContinuousColour MirrorBRDF::integrateHemisphere(RayTracer &tracer, const Scene &scene, const HitInfo &hit, unsigned recursionsSoFar, double rouletteValue) {
    if (tracer.stoppingCriterion(recursionsSoFar, 1.0))  // Still needed for recursion depth maximum.
        return {0,0,0};

    Ray ray = Ray(hit.hitpoint, reflect(hit.shadingNormal, hit.omega_o), All);
    bool incrementRecursionDepth = rouletteValue < tracer.rouletteChanceOfDying;  // I invented this.
    return tracer.trace(scene, ray, recursionsSoFar + incrementRecursionDepth);
}


//double schlick(const HitInfo& hit) {
//    double cos_phiO = hit.shadingNormal * hit.omega_o;
//    double R0 = ((n1 - n2)* (n1 - n2))/((n1 + n2)*(n1 + n2));
//    return R0 + (1 - R0)*(1 - pow(cos_phiO, 5));
//}

//FresnelBRDF::FresnelBRDF(bool opaque, double refractiveIndex) : opaque(opaque), refractiveIndex(refractiveIndex) {}
//
//ContinuousColour FresnelBRDF::integrateShadow(RayTracer &tracer, const Scene &scene, const HitInfo &hit) {
//    return {0,0,0};
//}
//
//// TODO:
////  - Also take into account TIR, which medium you go to/come from ...
////  - Extra difficult with infinitely thin objects, which can either be standalone or actually be part of a closed object
////    (e.g. a cube vs. a plane)
//ContinuousColour FresnelBRDF::integrateHemisphere(RayTracer &tracer, const Scene &scene, const HitInfo &hit, unsigned int recursionsSoFar) {
//    ContinuousColour totalRadiance;
//
//    // Reflection
//    double fresnelReflectance = schlick(hit);
//    Ray reflectedRay = Ray(hit.hitpoint, reflect(hit.shadingNormal, hit.omega_o), All);
//    totalRadiance += tracer.trace(scene, ray, recursionsSoFar+1);
//
//    // Transmission
//    if (!this->opaque) {
//        // https://physics.stackexchange.com/questions/435512/snells-law-in-vector-form
//        double mu = n_1/n_2;
//        double cos_phiO = hit.shadingNormal * hit.omega_o;
//        Vector3 t = (sqrt(1 - mu*mu*(1 - cos_phiO*cos_phiO)) - mu*cos_phiO) * hit.shadingNormal + mu * hit.omega_o;
//
//        Ray transmittedRay = Ray(hit.hitpoint, t, All);
//        totalRadiance += tracer.trace(scene, transmittedRay, recursionsSoFar+1);
//    }
//
//    return totalRadiance;
//}



///**
// * Glossy/specular/imperfect mirror
// */
//
//GlossyBRDF::GlossyBRDF(double exponent)
//    : exponent(exponent) {}


//double GlossyBRDF::evaluate(const Vector3 &p, const Vector3 &normal, const Vector3 &incomingRay, const Vector3 &outgoingRay) {
//    auto perfectReflection = reflect(normal, outgoingRay);
//    auto cosine = incomingRay * perfectReflection;
//    return std::pow(cosine, this->exponent);
//}
