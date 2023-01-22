#ifndef TREASACH_BRDF_H
#define TREASACH_BRDF_H


#include <vector>
#include "../geometry/Vector3.h"
#include "../rendering/Screen.h"

class BRDF {
    virtual void generateDirections(const Vector3& p, const Vector3& normal, const Vector3& outgoingRay, std::vector<Vector3>& rays) = 0;
    virtual double evaluate(const Vector3& p, const Vector3& normal, const Vector3& incomingRay, const Vector3& outgoingRay) = 0;
};


class DiffuseBRDF : public BRDF {
    void generateDirections(const Vector3 &p, const Vector3 &normal, const Vector3 &outgoingRay, std::vector<Vector3>& rays) override;
    double evaluate(const Vector3 &p, const Vector3 &normal, const Vector3 &incomingRay, const Vector3 &outgoingRay) override;
};


class MirrorBRDF : public BRDF {
    void generateDirections(const Vector3 &p, const Vector3 &normal, const Vector3 &outgoingRay, std::vector<Vector3>& rays) override;
    double evaluate(const Vector3 &p, const Vector3 &normal, const Vector3 &incomingRay, const Vector3 &outgoingRay) override;
};


class GlossyBRDF : public BRDF {
    double exponent;
    GlossyBRDF(double exponent);

    void generateDirections(const Vector3 &p, const Vector3 &normal, const Vector3 &outgoingRay, std::vector<Vector3>& rays) override;
    double evaluate(const Vector3 &p, const Vector3 &normal, const Vector3 &incomingRay, const Vector3 &outgoingRay) override;
};


class TransmissionBRDF : public BRDF {
    double refractiveIndex;
    TransmissionBRDF(double refractiveIndex);

    void generateDirections(const Vector3 &p, const Vector3 &normal, const Vector3 &outgoingRay, std::vector<Vector3>& rays) override;
    double evaluate(const Vector3 &p, const Vector3 &normal, const Vector3 &incomingRay, const Vector3 &outgoingRay) override;
};


#endif //TREASACH_BRDF_H
