#ifndef TREASACH_SPHEREMATH_H
#define TREASACH_SPHEREMATH_H

#include "Vector3.h"
#include "../noise/RNG.h"
#include "Constants.h"


Vector3 sphericalToCartesian(double theta, double phi, double radius=1.0);
double sampleHemisphere(RNG& rng, const Vector3& normal, Vector3& result);
double sampleHemisphereBiased(RNG& rng, const Vector3& normal, Vector3& result, double exponent=1.0);
Vector3 reflect(const Vector3& axis, const Vector3& direction);


#endif //TREASACH_SPHEREMATH_H
