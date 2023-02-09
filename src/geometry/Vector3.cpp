#include "Vector3.h"
#include <cmath>

Vector3::Vector3()
    : Vector3(0, 0, 0) {}

Vector3::Vector3(double x, double y, double z)
    : x(x), y(y), z(z) {}

double Vector3::norm() const {
//    return (*this) * (*this);
    return sqrt((*this) * (*this));
}

void Vector3::normalise() {
    auto scaleFactor = 1.0/this->norm();
    this->x *= scaleFactor;
    this->y *= scaleFactor;
    this->z *= scaleFactor;
}

Vector3 Vector3::normalised() const {
    auto scaleFactor = 1.0/this->norm();
    return scaleFactor * (*this);
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 operator-(const Vector3& v) {
    return {
        -v.x,
        -v.y,
        -v.z
    };
}

// Dot product
double operator*(const Vector3& v1, const Vector3& v2) {
    return v1.x*v2.x +  v1.y*v2.y + v1.z*v2.z;
}

// Cross product
Vector3 operator^(const Vector3& v1, const Vector3& v2) {
    return Vector3(
             v1.y * v2.z - v2.y * v1.z,
            -v1.x * v2.z + v2.x * v1.z,
             v1.x * v2.y - v2.x * v1.y);
}

// Scalar multiplication
Vector3 operator*(double a, const Vector3& v) {
    return Vector3(
            a*v.x,
            a*v.y,
            a*v.z);
}

