#ifndef TREASACH_VECTOR3_H
#define TREASACH_VECTOR3_H


class Vector3 {

    public:
        double x;
        double y;
        double z;

        Vector3();
        Vector3(double x, double y, double z);

        double norm() const;
        void normalise();
        Vector3 normalised() const;
};

Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v);
Vector3 operator*(double a, const Vector3& v);
Vector3 operator^(const Vector3& v1, const Vector3& v2);  // Cross product
double operator*(const Vector3& v1, const Vector3& v2);   // Dot product


#endif //TREASACH_VECTOR3_H
