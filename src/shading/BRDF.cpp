#include "BRDF.h"
#include "../rendering/Screen.h"
#include <cmath>

Vector3 reflect(const Vector3& axis, const Vector3& ray) {
    auto verticalComponent = (ray * axis) * axis;
    auto horizontalComponent = ray - verticalComponent;
    return ray - 2*horizontalComponent
}


void DiffuseBRDF::generateDirections(const Vector3 &p, const Vector3 &normal, const Vector3 &outgoingRay, std::vector<Vector3>& rays) {

}


double DiffuseBRDF::evaluate(const Vector3 &p, const Vector3 &normal, const Vector3 &incomingRay, const Vector3 &outgoingRay) {

}


void MirrorBRDF::generateDirections(const Vector3 &p, const Vector3 &normal, const Vector3 &outgoingRay, std::vector<Vector3>& rays) {
    rays.push_back(reflect(normal, outgoingRay));
}


double MirrorBRDF::evaluate(const Vector3 &p, const Vector3 &normal, const Vector3 &incomingRay, const Vector3 &outgoingRay) {

}


GlossyBRDF::GlossyBRDF(double exponent)
    : exponent(exponent) {}

void GlossyBRDF::generateDirections(const Vector3 &p, const Vector3 &normal, const Vector3 &outgoingRay, std::vector<Vector3> &rays) {

}

double GlossyBRDF::evaluate(const Vector3 &p, const Vector3 &normal, const Vector3 &incomingRay, const Vector3 &outgoingRay) {
    auto perfectReflection = reflect(normal, outgoingRay);
    auto cosine = incomingRay * perfectReflection;
    return std::pow(cosine, this->exponent);
}


TransmissionBRDF::TransmissionBRDF(double refractiveIndex)
    : refractiveIndex(refractiveIndex) {}

void TransmissionBRDF::generateDirections(const Vector3 &p, const Vector3 &normal, const Vector3 &outgoingRay, std::vector<Vector3> &rays) {

}

double TransmissionBRDF::evaluate(const Vector3 &p, const Vector3 &normal, const Vector3 &incomingRay, const Vector3 &outgoingRay) {

}
