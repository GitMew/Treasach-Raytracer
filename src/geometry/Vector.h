#ifndef TREASACH_VECTOR_H
#define TREASACH_VECTOR_H


class Vector {

    public:
        double x;
        double y;
        double z;

        Vector();
        Vector(double x, double y, double z);

        Vector cross(const Vector& v2) const;
        double norm() const;

    private:

};

Vector operator+(const Vector& v1, const Vector& v2);
Vector operator-(const Vector& v1, const Vector& v2);
double operator*(const Vector& v1, const Vector& v2);
Vector operator*(double a, const Vector& v);


#endif //TREASACH_VECTOR_H
