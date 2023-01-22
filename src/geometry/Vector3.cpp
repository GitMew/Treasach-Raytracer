#include "Vector3.h"

Vector3::Vector3()
    : Vector3(0, 0, 0) {}

Vector3::Vector3(double x, double y, double z)
    : x(x), y(y), z(z) {}

double Vector3::norm() const {
    return (*this) * (*this);
}

Vector3 Vector3::cross(const Vector3& v2) const {
    return Vector3(
            this->y * v2.z - v2.y * this->z,
            -this->x * v2.z + v2.x * this->z,
            this->x * v2.y - v2.x * this->y);
}

void Vector3::normalise() {
    auto scaleFactor = 1.0/this->norm();
    this->x *= scaleFactor;
    this->y *= scaleFactor;
    this->z *= scaleFactor;
}


Colour operator+(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

// Dot product
double operator*(const Vector3& v1, const Vector3& v2) {
    return v1.x*v2.x +  v1.y*v2.y + v1.z*v2.z;
}

// Scalar multiplication
Vector3 operator*(double a, const Vector3& v) {
    return Vector3(
            a*v.x,
            a*v.y,
            a*v.z);
}

