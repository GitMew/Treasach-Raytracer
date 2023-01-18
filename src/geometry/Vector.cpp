#include "Vector.h"

Vector::Vector()
    : Vector(0,0,0) {}

Vector::Vector(double x, double y, double z)
    : x(x), y(y), z(z) {}

double Vector::norm() const {
    return (*this) * (*this);
}

Vector Vector::cross(const Vector& v2) const {
    return Vector(
            this->y * v2.z - v2.y * this->z,
            -this->x * v2.z + v2.x * this->z,
            this->x * v2.y - v2.x * this->y);
}


Vector operator+(const Vector& v1, const Vector& v2) {
    return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector operator-(const Vector& v1, const Vector& v2) {
    return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

// Dot product
double operator*(const Vector& v1, const Vector& v2) {
    return v1.x*v2.x +  v1.y*v2.y + v1.z*v2.z;
}

// Scalar multiplication
Vector operator*(double a, const Vector& v) {
    return Vector(
            a*v.x,
            a*v.y,
            a*v.z);
}

