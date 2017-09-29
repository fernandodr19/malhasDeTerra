#ifndef VECTOR3D
#define VECTOR3D

#include <cmath>
#include <complex>
#include <assert.h>

template<class T>
class Vector3D
{
public:
    Vector3D() : m_x(0), m_y(0), m_z(0) {}
    Vector3D(T x, T y, T z = 0) : m_x(x), m_y(y), m_z(z) {}

    void setX(T x) { m_x = x; }
    void setY(T y) { m_y = y; }
    void setZ(T z) { m_z = z; }

    T x() const { return m_x; }
    T y() const { return m_y; }
    T z() const { return m_z; }

    T length() const {
        return std::sqrt(std::pow(std::abs(m_x), 2.0) + std::pow(std::abs(m_y), 2.0) + std::pow(std::abs(m_z), 2.0));
    }

    T length2D() const {
        return std::sqrt(std::pow(std::abs(m_x), 2.0) + std::pow(std::abs(m_y), 2.0));
    }

    T distanceTo(const Vector3D& v) const {
        return std::sqrt(std::pow(v.m_x - m_x, 2.0) + std::pow(v.m_y - m_y, 2.0) + std::pow(v.m_z - m_z, 2.0));
    }

    T distanceTo2D(const Vector3D& v) const {
        return std::sqrt(std::pow(v.m_x - m_x, 2.0) + std::pow(v.m_y - m_y, 2.0));
    }

    template<class R>
    T dotProduct(const Vector3D<R>& v) const {
        return m_x * v.x() + m_y * v.y() + m_z * v.z();
    }

    Vector3D crossProduct(const Vector3D& v) const {
        return Vector3D(m_y * v.m_z - m_z * v.m_y,
                        m_z * v.m_x - m_x * v.m_z,
                        m_x * v.m_y - m_y * v.m_x);
    }

    Vector3D perpendicular() const {
        Vector3D v;
        if(m_x != 0 || m_y != 0)
            v = Vector3D(0, 0, 1);
        else if(m_x != 0 || m_z != 0)
            v = Vector3D(0, 1, 0);
        else
            v = Vector3D(1, 0, 0);

        return crossProduct(v);
    }

    T &operator[](int i)
    {
        assert((unsigned int)(i) < 3u);
        switch(i) {
        case 0:
            return m_x;
        case 1:
            return m_y;
        case 2:
            return m_z;
        }
        static T x = T(0);
        return x;
    }

    T operator[](int i) const
    {
        assert((unsigned int)(i) < 3u);
        switch(i) {
        case 0:
            return m_x;
        case 1:
            return m_y;
        case 2:
            return m_z;
        }
        return T(0);
    }

    Vector3D operator+(const Vector3D& v) const { return Vector3D(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z); }
    Vector3D operator-(const Vector3D& v) const { return Vector3D(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z); }
    Vector3D operator*(double v) const { return Vector3D(m_x*v, m_y*v, m_z*v); }
    Vector3D<std::complex<double>> operator*(const std::complex<double>& v) const { return Vector3D<std::complex<double>>(m_x*v, m_y*v, m_z*v); }
    Vector3D operator/(double v) const { return Vector3D(m_x/v, m_y/v, m_z/v); }
    Vector3D<std::complex<double>> operator/(const std::complex<double>& v) const { return Vector3D<std::complex<double>>(m_x/v, m_y/v, m_z/v); }

    Vector3D& operator+=(const Vector3D& v) { m_x += v.m_x; m_y += v.m_y; m_z += v.m_z; return *this; }
    Vector3D& operator-=(const Vector3D& v) { m_x -= v.m_x; m_y -= v.m_y; m_z -= v.m_z; return *this; }    
    Vector3D& operator*=(double v) { m_x*=v; m_y*=v; m_z*=v; return *this; }
    Vector3D& operator/=(double v) { m_x/=v; m_y/=v; m_z/=v; return *this; }

    bool operator<(const Vector3D& v) const {
        if(m_x < v.m_x)
            return true;
        else if(m_x == v.m_x) {
            if(m_y < v.m_y)
                return true;
            else if(m_y == v.m_y)
                return m_z < v.m_z;
        }
        return false;
    }

    Vector3D& operator=(const Vector3D& v) { m_x = v.m_x; m_y = v.m_y; m_z = v.m_z; return *this; }
    bool operator==(const Vector3D& v) const { return m_x == v.m_x && m_y == v.m_y && m_z == v.m_z; }
    bool operator!=(const Vector3D& v) const { return m_x != v.m_x || m_y != v.m_y || m_z != v.m_z; }

    void rotateXY(const Vector3D& origin, double angle) {
        translate(-origin);
        rotateXY(angle);
        translate(origin);
    }

    void rotateXZ(const Vector3D& origin, double angle) {
        translate(-origin);
        rotateXZ(angle);
        translate(origin);
    }

    void rotateYZ(const Vector3D& origin, double angle) {
        translate(-origin);
        rotateYZ(angle);
        translate(origin);
    }

    Vector3D& rotatedXY(const Vector3D& origin, double angle) {
        rotateXY(origin, angle);
        return *this;
    }

    Vector3D& rotatedXZ(const Vector3D& origin, double angle) {
        rotateXZ(origin, angle);
        return *this;
    }

    Vector3D& rotatedYZ(const Vector3D& origin, double angle) {
        rotateYZ(origin, angle);
        return *this;
    }

    void rotateXY(double angle) {
        double sinAngle = std::sin(angle);
        double cosAngle = std::cos(angle);
        double x = m_x, y = m_y;
        m_x = cosAngle * x - sinAngle * y;
        m_y = sinAngle * x + cosAngle * y;
    }

    void rotateXZ(double angle) {
        double sinAngle = std::sin(angle);
        double cosAngle = std::cos(angle);
        double x = m_x, z = m_z;
        m_x = cosAngle * x + sinAngle * z;
        m_z = -sinAngle * x + cosAngle * z;
    }

    void rotateYZ(double angle) {
        double sinAngle = std::sin(angle);
        double cosAngle = std::cos(angle);
        double y = m_y, z = m_z;
        m_y = cosAngle * y - sinAngle * z;
        m_z = sinAngle * y + cosAngle * z;
    }

    Vector3D& rotatedXY(double angle) {
        rotateXY(angle);
        return *this;
    }

    Vector3D& rotatedXZ(double angle) {
        rotateXZ(angle);
        return *this;
    }

    Vector3D& rotatedYZ(double angle) {
        rotateYZ(angle);
        return *this;
    }

    void translate(const Vector3D& v) {
        m_x += v.m_x;
        m_y += v.m_y;
    }

    Vector3D& translated(const Vector3D& v) {
        translate(v);
        return *this;
    }

    void normalize() {
        T l = length();
        if(l == T(0))
            return;

        m_x /= l;
        m_y /= l;
        m_z /= l;
    }

    Vector3D normalized() {
        Vector3D v = *this;
        v.normalize();
        return v;
    }

private:
    T m_x;
    T m_y;
    T m_z;
};

typedef Vector3D<double> Vector3Dd;
typedef Vector3D<std::complex<double>> Vector3Dcd;

#endif // VECTOR3D

