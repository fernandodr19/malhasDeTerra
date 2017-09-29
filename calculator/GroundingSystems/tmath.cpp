#include <tmath.h>
#include <limits>
#include <QDebug>

std::complex<double> math::parallelImpedance(const std::complex<double>& r1, const std::complex<double>& r2)
{
    double inf = std::numeric_limits<double>::infinity();
    if(r1.real() == 0. || r2.real() == 0.)
        return 0.;
    if(r1.real() == inf)
        return r2;
    if(r2.real() == inf)
        return r1;
    return (r1 * r2) / (r1 + r2);
}

std::complex<double> math::seriesImpedance(const std::complex<double>& r1, const std::complex<double>& r2)
{
    return r1 + r2;
}

void math::boundToPosition(double& x1, double& y1, double& w, double& h, double px, double py)
{
    double x2 = x1 + w;
    double y2 = y1 + h;

    x1 = std::min(x1, px);
    y1 = std::min(y1, py);
    x2 = std::max(x2, px);
    y2 = std::max(y2, py);

    w = x2 - x1;
    h = y2 - y1;
}

int math::alternateIndex(int i)
{
    if(i % 2 == 0)
        return -i / 2;
    else
        return (i+1) / 2;
}

double math::getFieldEllipseMaximum(const Vector3Dcd& p)
{
    // Book: EPRI AC Transmission Line Reference Book - 200kV and Above, Third Edition
    // Chapter 7: Electric and Magnetic Fields
    // Page: 7-93

    double fxm = std::abs(p.x());
    double fxa = std::arg(p.x());
    double fym = std::abs(p.y());
    double fya = std::arg(p.y());
    double fzm = std::abs(p.z());
    double fza = std::arg(p.z());
    double wt = 0.5 * std::atan(-(fxm*fxm*std::sin(2*fxa) + fym*fym*std::sin(2*fya) + fzm*fzm*std::sin(2*fza)) /
                                (fxm*fxm*std::cos(2*fxa) + fym*fym*std::cos(2*fya) + fzm*fzm*std::cos(2*fza)));

    Vector3Dd et1(fxm*std::sin(wt + fxa), fym*std::sin(wt + fya), fzm*std::sin(wt + fza));
    wt += math::pi / 2;
    Vector3Dd et2(fxm*std::sin(wt + fxa), fym*std::sin(wt + fya), fzm*std::sin(wt + fza));

    return std::max(et1.length(), et2.length());
}

void math::kronReduction(Eigen::MatrixXcd& z, int nodes)
{
    // http://www.openelectrical.org/wiki/index.php?title=Kron_Reduction
    // Reduction of a Multi-conductor Line with Earth Wires

    int n = z.rows();
    Eigen::MatrixXcd za = z.block(0, 0, n-nodes, n-nodes);
    Eigen::MatrixXcd ze1 = z.block(0, n-nodes, n-nodes, nodes);
    Eigen::MatrixXcd ze2 = z.block(n-nodes, 0, nodes, n-nodes);
    Eigen::MatrixXcd zee = z.block(n-nodes, n-nodes, nodes, nodes);
    z = za - ze1 * zee.inverse() * ze2;
}

QVector<Vector3Dd> math::lineCircleIntersections(const Vector3Dd& lineA, const Vector3Dd& lineB, const Vector3Dd& circleCenter, double circleRadius)
{
    // http://math.stackexchange.com/questions/228841/how-do-i-calculate-the-intersections-of-a-straight-line-and-a-circle
    // lineA is a line point ; lineB is a vector parallel to the line
    QVector<Vector3Dd> ret;

    double a = lineB.dotProduct(lineB);
    double b = 2*(lineA-circleCenter).dotProduct(lineB);
    double c = (lineA-circleCenter).dotProduct(lineA-circleCenter) - circleRadius*circleRadius;

    double delta = b*b - 4 * a * c;
    if(delta < 0)
        return ret;

    double t1 = (-b + std::sqrt(delta)) / (2 * a);
    double t2 = (-b - std::sqrt(delta)) / (2 * a);

    Vector3Dd p1 = lineA + lineB * t1;
    Vector3Dd p2 = lineA + lineB * t2;
    ret.push_back(p1);
    if(p1 != p2)
        ret.push_back(p2);
    return ret;
}

QVector<Vector3Dd> math::circleCircleIntersections(const Vector3Dd& c1, double r1, const Vector3Dd& c2, double r2)
{
    // http://math.stackexchange.com/questions/256100/how-can-i-find-the-points-at-which-two-circles-intersect
    if(c1 == c2)
        return QVector<Vector3Dd>(); // infinite solutions, ignore

    auto fx = [](double y, double x1, double y1, double r1, double x2, double y2, double r2) {
        return (2*y*(y1-y2) + (r1*r1-r2*r2) - (x1*x1-x2*x2) - (y1*y1-y2*y2)) / (-2 * (x1-x2));
    };
    auto fy = [](double x, double x1, double y1, double r1, double x2, double y2, double r2) {
        return (2*x*(x1-x2) + (r1*r1-r2*r2) - (x1*x1-x2*x2) - (y1*y1-y2*y2)) / (-2 * (y1-y2));
    };

    Vector3Dd a, b;
    if(c1.x() == c2.x()) {
        a = Vector3Dd(0, fy(0, c1.x(), c1.y(), r1, c2.x(), c2.y(), r2));
        b = Vector3Dd(1, fy(1, c1.x(), c1.y(), r1, c2.x(), c2.y(), r2)) - a;
    }
    else {
        a = Vector3Dd(fx(0, c1.x(), c1.y(), r1, c2.x(), c2.y(), r2), 0);
        b = Vector3Dd(fx(1, c1.x(), c1.y(), r1, c2.x(), c2.y(), r2), 1) - a;
    }
    return lineCircleIntersections(a, b, c1, r1);
}

/*
   Calculate the line segment PaPb that is the shortest route between
   two lines Pa1Pa2 and Pb1Pb2. Calculate also the values of mua and mub where
      Pa = Pa1 + mua (Pa2 - Pa1)
      Pb = Pb1 + mub (Pb2 - Pb1)
   Return FALSE if no solution exists.
*/
bool math::lineLineShortestPath(const Vector3Dd& pa1, const Vector3Dd& pa2, const Vector3Dd& pb1, const Vector3Dd& pb2, Vector3Dd& pa, Vector3Dd& pb, double& mua, double& mub, bool& parallel, double precision)
{
    // http://paulbourke.net/geometry/pointlineplane/
    parallel = false;

    Vector3Dd pbDir = pb2 - pb1;
    if(std::abs(pbDir.x()) < precision && std::abs(pbDir.y()) < precision && std::abs(pbDir.z()) < precision)
        return false;

    Vector3Dd paDir = pa2 - pa1;
    if(std::abs(paDir.x()) < precision && std::abs(paDir.y()) < precision && std::abs(paDir.z()) < precision)
        return false;

    Vector3Dd pab1 = pa1 - pb1;

    double d1343 = pab1.dotProduct(pbDir);
    double d4321 = pbDir.dotProduct(paDir);
    double d1321 = pab1.dotProduct(paDir);
    double d4343 = pbDir.dotProduct(pbDir);
    double d2121 = paDir.dotProduct(paDir);

    double denom = d2121 * d4343 - d4321 * d4321;
    if(std::abs(denom) < precision) {
        parallel = true;
        return false;
    }
    double numer = d1343 * d4321 - d1321 * d4343;

    mua = numer / denom;
    mub = (d1343 + d4321 * mua) / d4343;

    pa = pa1 + paDir * mua;
    pb = pb1 + pbDir * mub;
    return true;
}

bool math::lineLineIntersection(const Vector3Dd& pa1, const Vector3Dd& pa2, const Vector3Dd& pb1, const Vector3Dd& pb2, Vector3Dd& pa, Vector3Dd& pb, double& mua, double& mub, double precision)
{
    bool parallel;
    bool hasPath = lineLineShortestPath(pa1, pa2, pb1, pb2, pa, pb, mua, mub, parallel);
    if(parallel) {
        Vector3Dd paDir = pa2 - pa1;
        Vector3Dd crossE = (pb1 - pa1).crossProduct(paDir);
        bool isCollinear = std::abs(crossE.x()) < precision && std::abs(crossE.y()) < precision && std::abs(crossE.z()) < precision;
        if(!isCollinear)
            return false;

        bool intersect = false;
        mua = (pb1 - pa1).dotProduct(paDir) / paDir.dotProduct(paDir);
        if(mua >= 0-precision && mua <= 1+precision) {
            pa = pa1 + paDir * mua;
            intersect = true;
        }
        else {
            mua = (pb2 - pa1).dotProduct(paDir) / paDir.dotProduct(paDir);
            if(mua >= 0-precision && mua <= 1+precision) {
                pa = pa1 + paDir * mua;
                intersect = true;
            }
            else {
                mua = 0;
                pa = pa1;
            }
        }

        Vector3Dd pbDir = pb2 - pb1;
        mub = (pa1 - pb1).dotProduct(pbDir) / pbDir.dotProduct(pbDir);
        if(mub >= 0-precision && mub <= 1+precision) {
            pb = pb1 + pbDir * mub;
            intersect = true;
        }
        else {
            mub = (pa2 - pb1).dotProduct(pbDir) / pbDir.dotProduct(pbDir);
            if(mub >= 0-precision && mub <= 1+precision) {
                pb = pb1 + pbDir * mub;
                intersect = true;
            }
            else {
                mub = 0;
                pb = pb1;
            }
        }

        return intersect;
    }

    if(!hasPath)
        return false;

    if(mua < 0-precision || mua > 1+precision || mub < 0-precision || mub > 1+precision)
        return false;

    return pa.distanceTo2D(pb) < precision;
}

void math::segmentSegmentShortestPath(const Vector3Dd& pa1, const Vector3Dd& pa2, const Vector3Dd& pb1, const Vector3Dd& pb2, Vector3Dd& pa, Vector3Dd& pb, double& mua, double& mub, double precision) {
    // http://geomalgorithms.com/a07-_distance.html#dist3D_Segment_to_Segment()
    Vector3Dd u = pa2 - pa1;
    Vector3Dd v = pb2 - pb1;
    Vector3Dd w = pa1 - pb1;

    double a = u.dotProduct(u);
    double b = u.dotProduct(v);
    double c = v.dotProduct(v);
    double d = u.dotProduct(w);
    double e = v.dotProduct(w);
    double D = a*c - b*b;
    double sc, sN, sD = D;
    double tc, tN, tD = D;

    if(D < precision) {
        sN = 0.0;
        sD = 1.0;
        tN = e;
        tD = c;
    } else {
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if(sN < 0.0) {
            sN = 0.0;
            tN = e;
            tD = c;
        } else if(sN > sD) {
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if(tN < 0.0) {
        tN = 0.0;

        if(-d < 0.0)
            sN = 0.0;
        else if(-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    } else if(tN > tD) {
        tN = tD;

        if((-d + b) < 0.0)
            sN = 0;
        else if((-d + b) > a)
            sN = sD;
        else {
            sN = (-d +  b);
            sD = a;
        }
    }

    sc = (std::abs(sN) < precision ? 0.0 : sN / sD);
    tc = (std::abs(tN) < precision ? 0.0 : tN / tD);

    mua = sc;
    mub = tc;

    pa = pa1 * (1.0 - sc) + pa2 * sc;
    pb = pb1 * (1.0 - tc) + pb2 * tc;
}
