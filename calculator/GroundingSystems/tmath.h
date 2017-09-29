#ifndef TMATH_H
#define TMATH_H

#include <complex>
#include <QVector>
#include "vector3d.h"
#include <Eigen/Dense>

template<class T>
class Profile
{
public:
    T pi;
    T pf;
    double precision;
};

namespace math
{
const double pi = 3.14159265358979323846264338327950288419717;
const double deg_to_rad = pi / 180.;
const double rad_to_deg = 180. / pi;
const double feet_to_meter = 0.3048;

const double c = 299792458; // speed of light
const double u0 = 4 * math::pi * 1E-07; // vaccum permeability
const double e0 = 8.854187817E-12; // vaccum permittivity
const double g = 9.80665; // gravitational acceleration

const double speedOfSound = 343.2; // m/s at 20C dry air
const double airDensity = 1.225; // kg/m^3 at 15C, 1atm
//const double airDensity = 1.204; // kg/m^3 at 20C, 1atm
const double earthRadius = 6371; // m

template<typename T>
T clamp(T x, T min, T max) { return std::max<T>(min, std::min<T>(x, max)); }

template<typename T, typename U>
T lerp(T a, T b, U t = 0.5) { return (1-t)*a + t*b; }

template<typename T>
T linearstep(T edge0, T edge1, T x) {
    x = clamp<T>((x - edge0) / (edge1 - edge0), 0, 1);
    return x;
}

template<typename T>
T smoothstep(T edge0, T edge1, T x) {
    x = clamp<T>((x - edge0) / (edge1 - edge0), 0, 1);
    return x*x*(3 - 2*x);
}

std::complex<double> parallelImpedance(const std::complex<double>& r1, const std::complex<double>& r2);
std::complex<double> seriesImpedance(const std::complex<double>& r1, const std::complex<double>& r2);

void boundToPosition(double& x1, double& y1, double& w, double& h, double px, double py);

int alternateIndex(int i);

double getFieldEllipseMaximum(const Vector3Dcd& p);

template<typename T>
double normalPDF(T x, T m, T s) { // Normal probability distribuition function
    return std::exp(-(x-m)*(x-m) / (2*s*s)) / (s * std::sqrt(2 * pi));
}

template<typename T>
double normalCDF(T x, T m, T sigma) { // Normal cumulative distribuition function
    return 0.5 * std::erfc(-(x - m) / (sigma * std::sqrt(2)));
}

void kronReduction(Eigen::MatrixXcd& z, int nodes);

QVector<Vector3Dd> lineCircleIntersections(const Vector3Dd& lineA, const Vector3Dd& lineB, const Vector3Dd& circleCenter, double circleRadius);
QVector<Vector3Dd> circleCircleIntersections(const Vector3Dd& c1, double r1, const Vector3Dd& c2, double r2);
bool lineLineShortestPath(const Vector3Dd& pa1, const Vector3Dd& pa2, const Vector3Dd& pb1, const Vector3Dd& pb2, Vector3Dd& pa, Vector3Dd& pb, double& mua, double& mub, bool& parallel, double precision = 1E-06);
bool lineLineIntersection(const Vector3Dd& pa1, const Vector3Dd& pa2, const Vector3Dd& pb1, const Vector3Dd& pb2, Vector3Dd& pa, Vector3Dd& pb, double& mua, double& mub, double precision = 1E-06);
void segmentSegmentShortestPath(const Vector3Dd& pa1, const Vector3Dd& pa2, const Vector3Dd& pb1, const Vector3Dd& pb2, Vector3Dd& pa, Vector3Dd& pb, double& mua, double& mub, double precision = 1E-06);

}

#endif // TMATH_H
