#ifndef TREASACH_VECTOR3_H
#define TREASACH_VECTOR3_H


class Vector3 {

    public:
        double x;
        double y;
        double z;

        Vector3();
        Vector3(double x, double y, double z);

        Vector3 cross(const Vector3& v2) const;
        double norm() const;
        void normalise();

    private:

};

Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator*(double a, const Vector3& v);
double operator*(const Vector3& v1, const Vector3& v2);


#endif //TREASACH_VECTOR3_H
