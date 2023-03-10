#include "SphereMath.h"
#include <cmath>


Vector3 sphericalToCartesian(double theta, double phi, double radius) {
    double reducedRadius = radius*sin(phi);
    return Vector3({
           reducedRadius*cos(theta),
           radius*cos(phi),
           reducedRadius*sin(theta)
   });
}


/**
 * To sample a hemisphere, I thought the process would go like this:
 *      1. Sample parameters (theta, phi) from [0,2pi[ x [0,pi/2].
 *      2. Generate ray direction as a point (x,y,z) on the unit sphere using the spherical->cartesian formulas.
 *      3. Rotate by the same rotation the normal had to undergo to go from straight up, i.e. (0,1,0), to where it is.
 *         Can probably be done using Rodrigues's rotation formula, see https://math.stackexchange.com/q/4632889/615621.
 *
 * Actually, the last step is different. Note how the ray direction is a point (x,y,z) that could be in world coords,
 * but you can also interpret its coordinate system as any coordinate system where the normal is the y axis. Hence, the
 * process is actually:
 *      3. Generate two tangents that form an orthogonal basis with the normal.
 *      4. Return x*tangent1 + y*normal + z*tangent2.
 * See equation (7.3) and listing 17.5 in Suffern.
 */
double sampleHemisphere(RNG& rng, const Vector3& normal, Vector3& result) {
    // 1.
    double p1 = rng.rand01();
    double p2 = rng.rand01();

    //
    double theta = 2*M_PI*p1;
    double phi   = M_PI_2*p2;
    double pdf   = M_1_PI*M_1_PI;
    //

    // 2.
    Vector3 coords = sphericalToCartesian(theta, phi);

    // 3. z = x ^ y;   x = y ^ z;   y = z ^ x
    Vector3 tangent1 = normal ^ CROSSPRODUCT_DUMMY;  // x is arbitrary. Assumption: normal and dummy are not parallel.
    tangent1.normalise();
    Vector3 tangent2 = tangent1 ^ normal;  // z = x ^ y. This is already normalised.

    // 4.
    result = coords.x*tangent1 + coords.y*normal + coords.z*tangent2;
    return pdf;
}

/**
 * Same as sampleHemisphere, but based on Suffern equation (7.2) instead.
 *
 * The sampled PDF is
 *      f(theta,phi) = f(theta)*f(phi)
 * with
 *      f(theta) = 1/2pi
 *      f(phi)   = (e+1) sin(phi) cos^e(phi)
 * whose cumulatives are
 *      F(theta) = theta/2pi
 *      F(phi)   = 1 - cos^(1+e)(phi)
 */
double sampleHemisphereBiased(RNG& rng, const Vector3& normal, Vector3& result, double exponent) {
    // 1.
    double p1 = rng.rand01();
    double p2 = rng.rand01();

    //
    double theta = 2*M_PI*p1;
    double phi   = acos(pow(1-p2, 1.0/(1+exponent)));
    double pdf   = (exponent+1)/(2*M_PI)*sin(phi)*pow(cos(phi), exponent);
    //

    // 2.
    Vector3 coords = sphericalToCartesian(theta, phi);

    // 3. z = x ^ y;   x = y ^ z;   y = z ^ x
    Vector3 tangent1 = normal ^ CROSSPRODUCT_DUMMY;  // x is arbitrary. Assumption: normal and dummy are not parallel.
    tangent1.normalise();
    Vector3 tangent2 = tangent1 ^ normal;  // z = x ^ y. This is already normalised.

    // 4.
    result = coords.x*tangent1 + coords.y*normal + coords.z*tangent2;
    return pdf;
}


/**
 * Precondition: all arguments must be normalised.
 */
Vector3 reflect(const Vector3& axis, const Vector3& direction) {
    auto verticalComponent = (direction * axis) * axis;
    auto horizontalComponent = direction - verticalComponent;
    return direction - 2 * horizontalComponent;
}