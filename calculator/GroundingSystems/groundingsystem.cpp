#include "groundingsystem.h"
#include <Eigen/Dense>
#include <Eigen/IterativeLinearSolvers>
#include <QDebug>
#include <QSettings>
#include <QTime>
#include <tmath.h>
#include <QFile>
#include <QTextCodec>
#include <cable.h>
#include <project.h>
#include <settings.h>
//#include <thirdparty/dxflib/dl_dxf.h>
//#include <thirdparty/dxflib/dl_creationadapter.h>

//#include <settings.h>
//#include <project.h>
//#include <database.h>

// References
// [1] COMPUTATION OF POTENTIAL AT SURFACE ABOVE AN ENERGIZED GRID OR OTHER ELECTRODE, ALLOWING FOR NON-UNIFORM CURRENT DISTRIBUTION
// Robert J. Heppe, 1979

// [2] STEP POTENTIALS AND BODY CURRENTS NEAR GROUNDS IN TWO-LAYER EARTH
// Robert J. Heppe, 1979

// [3] IEEE Std 80 - 2013

GroundingSystem::GroundingSystem()
{

}

void GroundingSystem::load(QSettings *settings)
{
    m_showPoints = settings->value("showPoints", false).toBool();
    m_showConductors = settings->value("showConductors", true).toBool();

    settings->beginGroup("Conductors");
    m_conductors.clear();
    QStringList conductors = settings->childGroups();
    for(const QString& conductorGroup : conductors) {
        settings->beginGroup(conductorGroup);
        Conductor conductor;
        conductor.p0.setX(settings->value("x0").toDouble());
        conductor.p0.setY(settings->value("y0").toDouble());
        conductor.p0.setZ(settings->value("z0").toDouble());
        conductor.p1.setX(settings->value("x1").toDouble());
        conductor.p1.setY(settings->value("y1").toDouble());
        conductor.p1.setZ(settings->value("z1").toDouble());

        int cableId = settings->value("cableId").toInt();
        conductor.cable = g_database->getCableById(cableId);

        if(!conductor.cable) {
            const QVector<CablePtr>& shieldWires = g_database->getCablesByCategory(CableCategory_ShieldWire);
            if(!shieldWires.empty())
                conductor.cable = shieldWires.first();
        }

        m_conductors.push_back(conductor);
        settings->endGroup();
    }
    settings->endGroup();

    settings->beginGroup("SurfaceVoltagePoints");
    m_surfaceVoltagePoints.clear();
    QStringList points = settings->childGroups();
    for(const QString& group : points) {
        settings->beginGroup(group);
        Vector3Dd point;
        point.setX(settings->value("x").toDouble());
        point.setY(settings->value("y").toDouble());
        m_surfaceVoltagePoints.push_back(point);
        settings->endGroup();
    }
    settings->endGroup();

    settings->beginGroup("SurfaceVoltageProfiles");
    m_surfaceVoltageProfiles.clear();
    QStringList profiles = settings->childGroups();
    for(const QString& group : profiles) {
        settings->beginGroup(group);
        SurfaceVoltageProfile<Vector3Dd> profile;
        profile.pi.setX(settings->value("x0").toDouble());
        profile.pi.setY(settings->value("y0").toDouble());
        profile.pf.setX(settings->value("x1").toDouble());
        profile.pf.setY(settings->value("y1").toDouble());
        profile.precision = settings->value("precision", 1).toDouble();
        profile.touch = settings->value("touch", true).toBool();
        profile.step = settings->value("step", true).toBool();
        m_surfaceVoltageProfiles.push_back(profile);
        settings->endGroup();
    }
    settings->endGroup();

    m_name = settings->value("name").toString();
    m_numberOfLayers = settings->value("numberOfLayers").toInt();
    m_segmentMaximumLength = settings->value("segmentMaximumLength").toString();
    m_crushedStoneLayerDepth = settings->value("crushedStoneLayerDepth").toString();
    m_crushedStoneLayerResistivity = settings->value("crushedStoneLayerResistivity").toString();
    m_firstLayerDepth = settings->value("firstLayerDepth").toString();
    m_firstLayerResistivity = settings->value("firstLayerResistivity").toString();
    m_secondLayerResistivity = settings->value("secondLayerResistivity").toString();
    m_injectedCurrent = settings->value("injectedCurrent").toString();
//    setDxfFile(settings->value("dxfFile").toString());
}

void GroundingSystem::save(QSettings *settings, bool saveTemporary)
{
    settings->beginGroup("Conductors");
    settings->setValue("_isArray", true);
    int conductorsCount = m_conductors.size();
    int tDigits = (int)std::log10(conductorsCount) + 1;
    for(int i = 0; i < conductorsCount; ++i) {
        Conductor& conductor = m_conductors[i];
        settings->beginGroup(QString("Conductor%1").arg(i, tDigits, 10, QChar('0')));
        settings->setValue("x0", conductor.p0.x());
        settings->setValue("y0", conductor.p0.y());
        settings->setValue("z0", conductor.p0.z());
        settings->setValue("x1", conductor.p1.x());
        settings->setValue("y1", conductor.p1.y());
        settings->setValue("z1", conductor.p1.z());
        if(conductor.cable)
            settings->setValue("typeId", conductor.cable->getId());
        if(saveTemporary)
            settings->setValue("length", conductor.length());
        settings->endGroup();
    }
    settings->endGroup();

    settings->beginGroup("SurfaceVoltagePoints");
    settings->setValue("_isArray", true);
    int pointsCount = m_surfaceVoltagePoints.size();
    int pDigits = (int)std::log10(pointsCount) + 1;
    for(int i = 0; i < pointsCount; ++i) {
        Vector3Dd& point = m_surfaceVoltagePoints[i];
        settings->beginGroup(QString("Point%1").arg(i, pDigits, 10, QChar('0')));
        settings->setValue("x", point.x());
        settings->setValue("y", point.y());
        settings->endGroup();
    }
    settings->endGroup();

    settings->beginGroup("SurfaceVoltageProfiles");
    settings->setValue("_isArray", true);
    int profilesCount = m_surfaceVoltageProfiles.size();
    int prDigits = (int)std::log10(profilesCount) + 1;
    for(int i = 0; i < profilesCount; ++i) {
        SurfaceVoltageProfile<Vector3Dd>& profile = m_surfaceVoltageProfiles[i];
        settings->beginGroup(QString("Profile%1").arg(i, prDigits, 10, QChar('0')));
        settings->setValue("x0", profile.pi.x());
        settings->setValue("y0", profile.pi.y());
        settings->setValue("x1", profile.pf.x());
        settings->setValue("y1", profile.pf.y());
        settings->setValue("precision", profile.precision);
        settings->setValue("touch", profile.touch);
        settings->setValue("step", profile.step);

        if(saveTemporary) {
            settings->setValue("maxTouchVoltage", profile.maxTouchVoltage);
            Settings::saveVector3D(settings, "maxTouchVoltagePos", profile.maxTouchVoltagePos);
            settings->setValue("maxStepVoltage", profile.maxStepVoltage);
            Settings::saveVector3D(settings, "maxStepVoltagePos1", profile.maxStepVoltagePos1);
            Settings::saveVector3D(settings, "maxStepVoltagePos2", profile.maxStepVoltagePos2);
        }
        settings->endGroup();
    }
    settings->endGroup();

    settings->setValue("name", m_name);
    settings->setValue("numberOfLayers", m_numberOfLayers);
    settings->setValue("segmentMaximumLength", m_segmentMaximumLength);
    settings->setValue("crushedStoneLayerDepth", m_crushedStoneLayerDepth);
    settings->setValue("crushedStoneLayerResistivity", m_crushedStoneLayerResistivity);
    settings->setValue("firstLayerDepth", m_firstLayerDepth);
    settings->setValue("firstLayerResistivity", m_firstLayerResistivity);
    settings->setValue("secondLayerResistivity", m_secondLayerResistivity);
    settings->setValue("injectedCurrent", m_injectedCurrent);
    settings->setValue("dxfFile", m_dxfFile);

    if(saveTemporary) {
//        settings->setValue("id", getId());
        settings->setValue("resistance", m_resistance);
    }
}

bool GroundingSystem::validate(QString& error)
{
    bool lengthWarning = false;
    for(int i = 0; i < m_conductors.size(); ++i) {
        Conductor& c = m_conductors[i];
        double length = c.p0.distanceTo(c.p1);
        if(length < 1E-06) {
            error += QObject::tr("Conductor %1 has length equals to 0.").arg(i+1) + getErrorLocation();
            return false;
        }
        else if(!lengthWarning && length > 1E04) {
            error += QObject::tr("Conductor %1 has length greater than 10,000 m.\n"
                                 "This is just a warning and calculate will happen. Check the scale.").arg(i+1) + getErrorLocation();
            lengthWarning = true;
        }
    }

    int disconnectedSegment;
    if(!isConnected(disconnectedSegment))
        error += QObject::tr("Assuming that line segment 1 of grounding system '%1' is connected, then line segment %2 is disconnected.\n"
                             "If you are sure that this configuration is equipotential, then the calculation will be done correctly.").arg(m_name).arg(disconnectedSegment+1) + "\n\n";

    for(int i = 0; i < m_surfaceVoltageProfiles.size(); ++i) {
        const SurfaceVoltageProfile<Vector3Dd>& profile = m_surfaceVoltageProfiles[i];
        if(profile.precision < 1E-03) {
            error += QObject::tr("The precision of surface voltage profile %1 is smaller than 0.001.").arg(i+1) + getErrorLocation();
            return false;
        }
    }

    bool ok;

    double maximumConductorSegmentLength = m_segmentMaximumLength.toDouble(&ok);
    if(!ok || maximumConductorSegmentLength <= 0) {
        error += QObject::tr("Maximum conductor segment length is invalid.") + getErrorLocation();
        return false;
    }

    double firstLayerResistivity = m_firstLayerResistivity.toDouble(&ok);
    if(!ok || firstLayerResistivity <= 0) {
        error += QObject::tr("Resistivity of soil's first layer is invalid.") + getErrorLocation();
        return false;
    }

    double crushedStoneLayerDepth = m_crushedStoneLayerDepth.toDouble(&ok);
    if(!ok || crushedStoneLayerDepth < 0) {
        error += QObject::tr("Depth of crushed stone layer is invalid.") + getErrorLocation();
        return false;
    }

    double crushedStoneLayerResistivity = m_crushedStoneLayerResistivity.toDouble(&ok);
    if(!ok || crushedStoneLayerResistivity <0) {
        error += QObject::tr("Resistivity of crushed stone layer is invalid.") + getErrorLocation();
        return false;
    }

    if(true/*m_project->hasModule(Application::Module_GroundingSystemInjectedCurrent)*/) {
        double injectedCurrent = m_injectedCurrent.toDouble(&ok);
        if(!ok || injectedCurrent < 0) {
            error += QObject::tr("Injected current is invalid.") + getErrorLocation();
            return false;
        }
    }

    return true;
}

void GroundingSystem::addConductor(const Vector3Dd& pos1, const Vector3Dd& pos2, const CablePtr& cable)
{
    Conductor c;
    c.p0 = pos1;
    c.p1 = pos2;
    c.cable = cable;
    m_conductors.push_back(c);
}

bool GroundingSystem::canMergeConductors(int& i, int& j)
{
    const double p = 1E-04;
    int n = m_conductors.size();
    for(i = 0; i < n; ++i) {
        Conductor& c1 = m_conductors[i];
        if(c1.length() < p)
            continue;

        Vector3Dd dir1 = (c1.p1 - c1.p0).normalized();
        for(j = i+1; j < n; ++j) {
            const Conductor& c2 = m_conductors[j];
            if(c2.length() < p)
                continue;

            // Check if c2.p0 and c2.p1 passes through infinite line c1
            Vector3Dd cross0 = dir1.crossProduct(c2.p0 - c1.p0); // check if c2.p0 passes through c1
            if(std::abs(cross0.x()) > p || std::abs(cross0.y()) > p || std::abs(cross0.z()) > p)
                continue;

            Vector3Dd cross1 = dir1.crossProduct(c2.p1 - c1.p0); // check if c2.p1 passes through c1
            if(std::abs(cross1.x()) > p || std::abs(cross1.y()) > p || std::abs(cross1.z()) > p)
                continue;

            // Check if c2.p0 or c2.p1 are connected to finite line c1
            double dot0 = dir1.dotProduct(c2.p0 - c1.p0);
            double dot1 = dir1.dotProduct(c2.p1 - c1.p0);
            if(dot0 < -p && dot1 < -p)
                continue;

            if(dot0 > c1.length() + p && dot1 > c1.length() + p)
                continue;

            return true;
        }
    }
    return false;
}

void GroundingSystem::mergeConductors()
{
    int i, j;
    while(canMergeConductors(i, j)) {
        Conductor& c1 = m_conductors[i];
        const Conductor& c2 = m_conductors[j];

        Vector3Dd dir1 = (c1.p1 - c1.p0).normalized();

        double dot0 = dir1.dotProduct(c2.p0 - c1.p0);
        if(dot0 < 0)
            c1.p0 = c2.p0;
        else if(dot0 > c1.length())
            c1.p1 = c2.p0;

        dir1 = (c1.p1 - c1.p0).normalized();

        double dot1 = dir1.dotProduct(c2.p1 - c1.p0);
        if(dot1 < 0)
            c1.p0 = c2.p1;
        else if(dot1 > c1.length())
            c1.p1 = c2.p1;

        m_conductors.erase(m_conductors.begin() + j);
    }
}

bool GroundingSystem::calculateResistance(QString& error)
{
    QTime t;
    t.start();

    m_surfaceVoltages.clear();
    m_resistance = -1;

    calculateSegments();

    int size = m_segments.size();
    if(m_segments.empty()) {
        error += QObject::tr("There are no segments.") + "\n";
        return false;
    }

    // Reflection factor. [1] page 1986
    m_k = (getSecondLayerResistivity() - getFirstLayerResistivity()) / (getSecondLayerResistivity() + getFirstLayerResistivity());

    // [1] Page 1979. Eq. 1
    double v0 = 1; // Arbitrary potential
    Eigen::MatrixXd V = Eigen::MatrixXd::Constant(size, 1, v0);
    Eigen::MatrixXd R(size, size);

    unsigned int current = 0;

    for(int i = 0; i < size; ++i) {
        const ConductorSegment& c1 = m_segments[i];

        for(int j = i; j < size; ++j) {
            ++current;

            const ConductorSegment& c2 = m_segments[j];

            double r = 0;
            if(!calculateMutualResistance(c1, c2, r, error))
                return false;
            R(i, j) = r;
            if(i != j)
                R(j, i) = r;
        }
    }

    t.restart();

    Eigen::ConjugateGradient<Eigen::MatrixXd, Eigen::Lower | Eigen::Upper> cg(R);
    Eigen::MatrixXd I = cg.solve(V);

    QSet<int> checkedConductors;
    double sumI = 0;
    for(int i = 0; i < size; ++i) {
        ConductorSegment& cs = m_segments[i];
        cs.current1V = I(i, 0);
        sumI += cs.current1V;
        if(cs.current1V <= 0 && !checkedConductors.contains(cs.conductorIndex)) {
            error += QObject::tr("The current on conductor %1 is below 0. Check for conductor superposition.").arg(cs.conductorIndex+1) + "\n";
            checkedConductors.insert(cs.conductorIndex);

            //            for(int j = 0; j < size; ++j) {
            //                ConductorSegment& c2 = m_segments[i];
            //                double m = calculateM(cs, c2);
            //                double mn = calculateNumericalM(cs, c2);
            //                if(100 * std::abs(m - mn) / mn > 5) {
            //                    qDebug() << "Segment" << i << m << mn;
            //                    qDebug() << cs.averageZ << cs.radius << cs.length << cs.dir.x() << cs.dir.y() << cs.dir.z();
            //                    qDebug() << c2.averageZ << c2.radius << c2.length << c2.dir.x() << c2.dir.y() << c2.dir.z();

            //                    qDebug() << cs.a.x() << cs.b.x() << cs.a.y() << cs.b.y() << cs.a.z() << cs.b.z() << cs.conductorIndex;
            //                    qDebug() << c2.a.x() << c2.b.x() << c2.a.y() << c2.b.y() << c2.a.z() << c2.b.z() << c2.conductorIndex;
            //                }
            //            }
        }
    }

    m_resistance = v0 / sumI;
    return true;
}

double GroundingSystem::calculateSurfaceVoltage(const Vector3Dd& p)
{
    if(p.z() != 0)
        return 0;

    double im = getInjectedCurrent();
    double vm = m_resistance * im;

    double v = 0;

    double h = getFirstLayerDepth();

    if(m_numberOfLayers > 1) { // Two layers
        for(const ConductorSegment& cs : m_segments) {
            if(cs.averageZ >= -h) { // First layer
                double sum1 = 0;
                for(int n = 1; ; ++n) {
                    ConductorSegment cil1 = cs;
                    cil1.a.setZ(2*n*h - cil1.a.z());
                    cil1.b.setZ(2*n*h - cil1.b.z());
                    cil1.dir = (cil1.b - cil1.a).normalized();
                    cil1.averageZ = (cil1.a.z() + cil1.b.z()) / 2.;

                    ConductorSegment cil2 = cs;
                    cil2.a.setZ(2*n*h + cil2.a.z());
                    cil2.b.setZ(2*n*h + cil2.b.z());
                    cil2.dir = (cil2.b - cil2.a).normalized();
                    cil2.averageZ = (cil2.a.z() + cil2.b.z()) / 2.;

                    double factor = std::pow(m_k, n) * (calculateSegmentVoltage(cil1, p) + calculateSegmentVoltage(cil2, p));
                    sum1 += factor;

                    if(std::isnan(factor) || n >= 5000)
                        return std::nan("");

                    if(n > 2 && std::abs(factor / (sum1 - factor)) <= 5E-03)
                        break;
                }

                // [2] Eq. 2
                v += 2. * vm * getFirstLayerResistivity() * (calculateSegmentVoltage(cs, p) + sum1) / cs.length;
            }
            else { // Second layer
                double sum1 = 0;
                for(int n = 1; ; ++n) {
                    ConductorSegment cil1 = cs;
                    cil1.a.setZ(2*n*h - cil1.a.z());
                    cil1.b.setZ(2*n*h - cil1.b.z());
                    cil1.dir = (cil1.b - cil1.a).normalized();
                    cil1.averageZ = (cil1.a.z() + cil1.b.z()) / 2.;

                    double factor = std::pow(m_k, n) * (calculateSegmentVoltage(cil1, p));
                    sum1 += factor;

                    if(std::isnan(factor) || n >= 5000)
                        return std::nan("");

                    if(n > 2 && std::abs(factor / (sum1 - factor)) <= 5E-03)
                        break;
                }

                // [2] Eq. 1
                v += 2. * vm * getSecondLayerResistivity() * (1. - m_k) * (calculateSegmentVoltage(cs, p) + sum1) / cs.length;
            }
        }
    }
    else {
        for(const ConductorSegment& cs : m_segments)
            v += 2. * vm * getFirstLayerResistivity() * calculateSegmentVoltage(cs, p) / cs.length;
    }
    return v;
}

double GroundingSystem::calculateTouchPotentialLimit1986(double faultTime, int weight)
{
    double rhoB = getCrushedStoneLayerResistivity();
    double rhoS = getFirstLayerResistivity();
    double k = (rhoS - rhoB) / (rhoS + rhoB);

    const double b = 0.08; // foot radius (m)
    const double s = 1; // feet distance (m)
    double hs = getCrushedStoneLayerDepth();

    // [2] Eq. 9
    double sum1 = 0;
    for(int n = 1; ; ++n) {
        double factor = std::pow(k, n) / std::sqrt(1 + std::pow(2 * n * hs / b, 2));
        sum1 += factor;

        if(std::isnan(factor) || n >= 5000)
            return std::nan("");

        if(n > 2 && std::abs(factor / (sum1 - factor)) <= 5E-03)
            break;
    }
    double rp = rhoB / (4 * b) * (1 + 2 * sum1);

    double sum2 = 0;
    for(int n = 1; ; ++n) {
        double factor = std::pow(k, n) / std::sqrt(1 + std::pow(2 * n * hs / s, 2));
        sum2 += factor;

        if(std::isnan(factor) || n >= 5000)
            return std::nan("");

        if(n > 2 && std::abs(factor / (sum2 - factor)) <= 5E-03)
            break;
    }
    double rm = rhoB / (4 * s) * (1 + 2 * sum2);

    const double bodyResistance = 1000; // ohm
    double rTouch = bodyResistance + 0.5 * (rp + rm);

    double kw;
    if(weight == 50)
        kw = 0.116;
    else if(weight == 70)
        kw = 0.157;
    else
        return std::nan("");

    double currentLimit = kw / std::sqrt(faultTime);
    double vLimit = currentLimit * rTouch;
    return vLimit;
}

double GroundingSystem::calculateStepPotentialLimit1986(double faultTime, int weight)
{
    double rhoB = getCrushedStoneLayerResistivity();
    double rhoS = getFirstLayerResistivity();
    double k = (rhoS - rhoB) / (rhoS + rhoB);

    const double b = 0.08; // foot radius (m)
    const double s = 1; // feet distance (m)
    double hs = getCrushedStoneLayerDepth();

    // [2] Eq. 9
    double sum1 = 0;
    for(int n = 1; ; ++n) {
        double factor = std::pow(k, n) / std::sqrt(1 + std::pow(2 * n * hs / b, 2));
        sum1 += factor;

        if(std::isnan(factor) || n >= 5000)
            return std::nan("");

        if(n > 2 && std::abs(factor / (sum1 - factor)) <= 5E-03)
            break;
    }
    double rp = rhoB / (4 * b) * (1 + 2 * sum1);

    double sum2 = 0;
    for(int n = 1; ; ++n) {
        double factor = std::pow(k, n) / std::sqrt(1 + std::pow(2 * n * hs / s, 2));
        sum2 += factor;

        if(std::isnan(factor) || n >= 5000)
            return std::nan("");

        if(n > 2 && std::abs(factor / (sum2 - factor)) <= 5E-03)
            break;
    }
    double rm = rhoB / (4 * s) * (1 + 2 * sum2);

    const double bodyResistance = 1000; // ohm
    double rStep = bodyResistance + 2 * (rp - rm);

    double kw;
    if(weight == 50)
        kw = 0.116;
    else if(weight == 70)
        kw = 0.157;
    else
        return std::nan("");

    double currentLimit = kw / std::sqrt(faultTime);

    // [2] Eq. 8
    double vLimit = currentLimit * rStep;
    return vLimit;
}

double GroundingSystem::calculateTouchPotentialLimit2013(double faultTime, int weight)
{
    // [3] Cap 7 and 8
    double rhoS, rho, hs;
    if(getCrushedStoneLayerDepth() > 0) {
        rhoS = getCrushedStoneLayerResistivity();
        rho = getFirstLayerResistivity();
        hs = getCrushedStoneLayerDepth();
    }
    else if(getNumberOfLayers() == 2) {
        rhoS = getFirstLayerResistivity();
        rho = getSecondLayerResistivity();
        hs = getFirstLayerDepth();
    }
    else {
        rhoS = getFirstLayerResistivity();
        rho = getFirstLayerResistivity();
        hs = 0;
    }
    double cs = 1 - ((0.09 * (1 - rho / rhoS)) / (2 * hs + 0.09));
    double kw;
    if(weight == 50)
        kw = 0.116;
    else if(weight == 70)
        kw = 0.157;
    else
        return std::nan("");

    return (1000 + 1.5 * cs * rhoS) * kw / std::sqrt(faultTime);
}

double GroundingSystem::calculateStepPotentialLimit2013(double faultTime, int weight)
{
    // [3] Cap 7 and 8
    double rhoS, rho, hs;
    if(getCrushedStoneLayerDepth() > 0) {
        rhoS = getCrushedStoneLayerResistivity();
        rho = getFirstLayerResistivity();
        hs = getCrushedStoneLayerDepth();
    }
    else if(getNumberOfLayers() == 2) {
        rhoS = getFirstLayerResistivity();
        rho = getSecondLayerResistivity();
        hs = getFirstLayerDepth();
    }
    else {
        rhoS = getFirstLayerResistivity();
        rho = getFirstLayerResistivity();
        hs = 0;
    }
    double cs = 1 - ((0.09 * (1 - rho / rhoS)) / (2 * hs + 0.09));
    double kw;
    if(weight == 50)
        kw = 0.116;
    else if(weight == 70)
        kw = 0.157;
    else
        return std::nan("");

    return (1000 + 6 * cs * rhoS) * kw / std::sqrt(faultTime);
}

bool GroundingSystem::isConnected(int& disconnectedSegment)
{
    int n = m_conductors.size();
    if(n == 0)
        return true;

    QVector<bool> visited(n, false);

    std::function<void(int)> visit = [&](int i) {
        visited[i] = true;

        const Conductor& c1 = m_conductors[i];
        for(int j = 0; j < n; ++j) {
            const Conductor& c2 = m_conductors[j];
            if(!visited[j]) {
                Vector3Dd pa, pb;
                double mua, mub;
                if(math::lineLineIntersection(c1.p0, c1.p1, c2.p0, c2.p1, pa, pb, mua, mub, 1E-03))
                    visit(j);
            }
        }
    };

    visit(0);

    for(int i = 0; i < n; ++i) {
        if(!visited[i]) {
            disconnectedSegment = i;
            return false;
        }
    }
    return true;
}

double GroundingSystem::getSurfaceVoltage(const Vector3Dd& p)
{
    Vector3Dd roundedP; // Use millimiter precision
    roundedP.setX(std::round((p.x() * 1000.)) / 1000.);
    roundedP.setY(std::round((p.y() * 1000.)) / 1000.);
    roundedP.setZ(std::round((p.z() * 1000.)) / 1000.);

    auto it = m_surfaceVoltages.find(roundedP);
    if(it != m_surfaceVoltages.end())
        return it.value();

    double v = calculateSurfaceVoltage(roundedP);
    m_surfaceVoltages[roundedP] = v;
    return v;
}

double GroundingSystem::calculateSegmentVoltage(const ConductorSegment& cs, const Vector3Dd& p)
{
    // [2] Page 54, Effect of a Point Source in the Top Layer on a Point in the Top Layer
    // Integral solved for theta, using same idea of https://pt.wikipedia.org/wiki/Lei_de_Biot-Savart
    // For a rectilinear wire
    double v = cs.current1V / (4 * math::pi);

    Vector3Dd c = cs.a + cs.dir * cs.dir.dotProduct(p - cs.a);

    double da = (cs.a - c).dotProduct(cs.dir);
    double db = (cs.b - c).dotProduct(cs.dir);

    double sinTheta1 = da / p.distanceTo(cs.a);
    double sinTheta2 = db / p.distanceTo(cs.b);
    if(1 - std::abs(sinTheta1) <= 1E-06 || 1 - std::abs(sinTheta2) <= 1E-06) { // P is at the AB line. Offset AB to radius will not change the solution significantly.
        Vector3Dd abP = (cs.b - cs.a).perpendicular().normalized() * cs.radius;
        Vector3Dd a = cs.a + abP;
        Vector3Dd b = cs.b + abP;

        da = (a - c).dotProduct(cs.dir);
        db = (b - c).dotProduct(cs.dir);
        sinTheta1 = da / p.distanceTo(a);
        sinTheta2 = db / p.distanceTo(b);
    }

    double cosTheta1 = std::sqrt(1 - sinTheta1*sinTheta1);
    double cosTheta2 = std::sqrt(1 - sinTheta2*sinTheta2);
    v *= std::log(((sinTheta2 + 1.) * cosTheta1) / ((sinTheta1 + 1.) * cosTheta2));

    //    // numerical
    //    double sum = 0;
    //    double dl = cs.length / 1000.;
    //    for(double l = 0; l <= cs.length; l += dl) {
    //        Vector3Dd cx = cs.a + cs.dir * l;
    //        sum += dl / (std::sqrt(std::pow(cx.x() - p.x(), 2) + std::pow(cx.y() - p.y(), 2) + std::pow(cx.z() - p.z(), 2)));
    //    }
    //    v *= sum;

    return v;
}

bool GroundingSystem::calculateMutualResistance(const ConductorSegment& ci, const ConductorSegment& cj, double& r, QString& error)
{
    // [1] Eq. A-5 (m depends on soil layers, later multiplied)
    r = 1. / (4. * math::pi * ci.length * cj.length);

    double h = getFirstLayerDepth();
    if(m_numberOfLayers > 1) { // Two layers. [1] Page 1986
        if(ci.averageZ >= -h && cj.averageZ >= -h) { // ci and cj on first layer
            r *= getFirstLayerResistivity();

            double sum0 = 0;
            double sum1 = 0;
            for(int n = 0; ; ++n) {
                ConductorSegment cil1 = ci;
                cil1.a.setZ(2*n*h - cil1.a.z());
                cil1.b.setZ(2*n*h - cil1.b.z());
                cil1.dir = (cil1.b - cil1.a).normalized();
                cil1.averageZ = (cil1.a.z() + cil1.b.z()) / 2.;

                ConductorSegment cil2 = ci;
                cil2.a.setZ(2*n*h + cil2.a.z());
                cil2.b.setZ(2*n*h + cil2.b.z());
                cil2.dir = (cil2.b - cil2.a).normalized();
                cil2.averageZ = (cil2.a.z() + cil2.b.z()) / 2.;

                double factor = std::pow(m_k, n) * (calculateM(cil1, cj) + calculateM(cil2, cj));
                sum0 += factor;

                if(std::isnan(factor) || n >= 5000) {
                    error += QObject::tr("Cant converge resistances.") + "\n";
                    return false;
                }

                if(n > 1 && std::abs(factor / (sum0 - factor)) <= 5E-03)
                    break;
            }

            for(int n = 1; ; ++n) {
                ConductorSegment cil1 = ci;
                cil1.a.setZ(-2*n*h - cil1.a.z());
                cil1.b.setZ(-2*n*h - cil1.b.z());
                cil1.dir = (cil1.b - cil1.a).normalized();
                cil1.averageZ = (cil1.a.z() + cil1.b.z()) / 2.;

                ConductorSegment cil2 = ci;
                cil2.a.setZ(-2*n*h + cil2.a.z());
                cil2.b.setZ(-2*n*h + cil2.b.z());
                cil2.dir = (cil2.b - cil2.a).normalized();
                cil2.averageZ = (cil2.a.z() + cil2.b.z()) / 2.;

                double factor = std::pow(m_k, n) * (calculateM(cil1, cj) + calculateM(cil2, cj));
                sum1 += factor;

                if(std::isnan(factor) || n >= 5000) {
                    error += QObject::tr("Cant converge resistances.") + "\n";
                    return false;
                }

                if(n > 2 && std::abs(factor / (sum1 - factor)) <= 5E-03)
                    break;
            }

            r *= (sum0 + sum1);
        }
        else if(ci.averageZ < -h && cj.averageZ < -h) { // ci and cj on second layer
            r *= getSecondLayerResistivity();

            double sum = 0;
            for(int n = 0; ; ++n) {
                ConductorSegment cil1 = ci;
                cil1.a.setZ(2*n*h - cil1.a.z());
                cil1.b.setZ(2*n*h - cil1.b.z());
                cil1.dir = (cil1.b - cil1.a).normalized();
                cil1.averageZ = (cil1.a.z() + cil1.b.z()) / 2.;

                double factor = std::pow(m_k, n) * calculateM(cil1, cj);
                sum += factor;

                if(std::isnan(factor) || n >= 5000) {
                    error += QObject::tr("Cant converge resistances.") + "\n";
                    return false;
                }

                if(n > 1 && std::abs(factor / (sum - factor)) <= 5E-03)
                    break;
            }

            ConductorSegment cil1 = ci;
            cil1.a.setZ(-2*h - cil1.a.z());
            cil1.b.setZ(-2*h - cil1.b.z());
            cil1.dir = (cil1.b - cil1.a).normalized();
            cil1.averageZ = (cil1.a.z() + cil1.b.z()) / 2.;

            double m = m_k * calculateM(cil1, cj);

            r *= calculateM(ci, cj) - m + (1-m_k*m_k) * sum;
        }
        else if(ci.averageZ >= -h && cj.averageZ < -h) { // ci on first layer, cj on second layer
            r *= getFirstLayerResistivity() * (1. + m_k);

            double sum0 = 0;
            double sum1 = 0;
            for(int n = 0; ; ++n) {
                ConductorSegment cil1 = ci;
                cil1.a.setZ(2*n*h - cil1.a.z());
                cil1.b.setZ(2*n*h - cil1.b.z());
                cil1.dir = (cil1.b - cil1.a).normalized();
                cil1.averageZ = (cil1.a.z() + cil1.b.z()) / 2.;

                double factor = std::pow(m_k, n) * calculateM(cil1, cj);
                sum0 += factor;

                if(std::isnan(factor) || n >= 5000) {
                    error += QObject::tr("Cant converge resistances.") + "\n";
                    return false;
                }

                if(n > 1 && std::abs(factor / (sum0 - factor)) <= 5E-03)
                    break;
            }

            for(int n = 1; ; ++n) {
                ConductorSegment cil1 = ci;
                cil1.a.setZ(2*n*h + cil1.a.z());
                cil1.b.setZ(2*n*h + cil1.b.z());
                cil1.dir = (cil1.b - cil1.a).normalized();
                cil1.averageZ = (cil1.a.z() + cil1.b.z()) / 2.;

                double factor = std::pow(m_k, n) * calculateM(cil1, cj);
                sum1 += factor;

                if(std::isnan(factor) || n >= 5000) {
                    error += QObject::tr("Cant converge resistances.") + "\n";
                    return false;
                }

                if(n > 2 && std::abs(factor / (sum1 - factor)) <= 5E-03)
                    break;
            }

            r *= (calculateM(ci, cj) + sum0 + sum1);

        }
        else if(ci.averageZ < -h && cj.averageZ >= -h) { // ci on second layer, cj on first layer
            r *= getSecondLayerResistivity() * (1. - m_k);

            double sum0 = 0;
            double sum1 = 0;
            for(int n = 0; ; ++n) {
                ConductorSegment cil1 = ci;
                cil1.a.setZ(2*n*h - cil1.a.z());
                cil1.b.setZ(2*n*h - cil1.b.z());
                cil1.dir = (cil1.b - cil1.a).normalized();
                cil1.averageZ = (cil1.a.z() + cil1.b.z()) / 2.;

                double factor = std::pow(m_k, n) * calculateM(cil1, cj);
                sum0 += factor;

                if(std::isnan(factor) || n >= 5000) {
                    error += QObject::tr("Cant converge resistances.") + "\n";
                    return false;
                }

                if(n > 1 && std::abs(factor / (sum0 - factor)) <= 5E-03)
                    break;
            }

            for(int n = 1; ; ++n) {
                ConductorSegment cil1 = ci;
                cil1.a.setZ(-2*n*h + cil1.a.z());
                cil1.b.setZ(-2*n*h + cil1.b.z());
                cil1.dir = (cil1.b - cil1.a).normalized();
                cil1.averageZ = (cil1.a.z() + cil1.b.z()) / 2.;

                double factor = std::pow(m_k, n) * calculateM(cil1, cj);
                sum1 += factor;

                if(std::isnan(factor) || n >= 5000) {
                    error += QObject::tr("Cant converge resistances.") + "\n";
                    return false;
                }

                if(n > 2 && std::abs(factor / (sum1 - factor)) <= 5E-03)
                    break;
            }

            r *= (calculateM(ci, cj) + sum0 + sum1);
        }
        else {
            error += QObject::tr("Cant calculate resistances at these positions.") + "\n";
            return 0;
        }
    }
    else { // Only one layer
        ConductorSegment cjl = cj;
        cjl.a.setZ(-cj.a.z());
        cjl.b.setZ(-cj.b.z());
        cjl.dir = (cjl.b - cjl.a).normalized();
        cjl.averageZ = (cjl.a.z() + cjl.b.z()) / 2.;

        double m = calculateM(ci, cj);
        double ml = calculateM(ci, cjl);
        r *= getFirstLayerResistivity() * (m + ml);
    }

    return true;
}

double GroundingSystem::calculateNumericalM(const ConductorSegment& c1, const ConductorSegment& c2)
{
    ConductorSegment c = c1;
    if(c1 == c2) {
        Vector3Dd abP = (c.b - c.a).perpendicular().normalized();
        c.a += abP * c.radius;
        c.b += abP * c.radius;
    }

    double m = 0;
    double oldM = 0;
    int tries = 0;
    int n = 16;
    do {
        oldM = m;
        m = 0;

        double dl1 = c.length / n;
        double dl2 = c2.length / n;

        for(double l1 = 0; l1 <= c.length; l1 += dl1) {
            Vector3Dd p1 = c.a + c.dir * l1;
            for(double l2 = 0; l2 <= c2.length; l2 += dl2) {
                Vector3Dd p2 = c2.a + c2.dir * l2;
                double r = std::max({p1.distanceTo(p2), c.radius, c2.radius});
                m += dl1 * dl2 / r;
            }
        }
        ++tries;
        if(tries > 16) {
            qCritical() << "Could not converge mutual." << n << m << oldM;
            break;
        }
        n *= 2;
    } while(tries < 2 || std::abs((m - oldM) / oldM) > 1E-2);

    return m;
}

double GroundingSystem::calculateM(const ConductorSegment& c1, const ConductorSegment& c2)
{
    // [1] Eq. A-2
    const double precision = 1E-10;

    Vector3Dd a = c1.a;
    Vector3Dd b = c1.b;
    Vector3Dd e = c2.a;
    Vector3Dd f = c2.b;

    const Vector3Dd& abDir = c1.dir;
    const Vector3Dd& efDir = c2.dir;

    double cosTheta = std::max(-1., std::min(1., abDir.dotProduct(efDir)));

    double absCosTheta = std::abs(cosTheta);
    bool isOrthogonal = absCosTheta < precision;
    bool isParallel = absCosTheta > (1 - precision);

    if(isParallel) {
        Vector3Dd crossE = abDir.crossProduct(e - a);

        bool isCollinear = std::abs(crossE.x()) < precision && std::abs(crossE.y()) < precision && std::abs(crossE.z()) < precision;
        if(isCollinear) {
            Vector3Dd crossF = abDir.crossProduct(f - a);
            isCollinear = isCollinear && std::abs(crossF.x()) < precision && std::abs(crossF.y()) < precision && std::abs(crossF.z()) < precision;
            if(isCollinear) {
                // If it's not the same conductor, this is parallel colinear. [1] appendix, case 4.
                // If it's the same conductor, this is self resistance. [1] appendix, case 5.
                // Mutual resistance with another conductor offseted by radius.
                Vector3Dd abP = (b - a).perpendicular().normalized();
                a += abP * c1.radius;
                b += abP * c1.radius;
            }
        }
    }

    Vector3Dd al = e + efDir * (a - e).dotProduct(efDir);
    Vector3Dd bl = e + efDir * (b - e).dotProduct(efDir);
    Vector3Dd el = a + abDir * (e - a).dotProduct(abDir);
    Vector3Dd fl = a + abDir * (f - a).dotProduct(abDir);

    Vector3Dd c;
    Vector3Dd g;

    if(isParallel) {
        c = a;
        g = al;
    }
    else if(isOrthogonal) {
        c = el;
        g = al;
    }
    else {
        Vector3Dd abCrossEf = abDir.crossProduct(efDir);
        Vector3Dd u = abDir.crossProduct(abCrossEf);
        double dotU = u.dotProduct(efDir);
        if(std::abs(dotU) < precision)
            g = e;
        else
            g = e + efDir * (u.dotProduct(a - e) / dotU);

        Vector3Dd w = efDir.crossProduct(abCrossEf);
        double dotW = w.dotProduct(abDir);
        if(std::abs(dotW) < precision)
            c = a;
        else
            c = a + abDir * (w.dotProduct(e - a) / dotW);
    }

    double ae = a.distanceTo(e);
    double af = a.distanceTo(f);
    double be = b.distanceTo(e);
    double bf = b.distanceTo(f);

    double ale = (e - al).dotProduct(efDir);
    double alf = (f - al).dotProduct(efDir);
    double ble = (e - bl).dotProduct(efDir);
    double blf = (f - bl).dotProduct(efDir);

    double ela = (a - el).dotProduct(abDir);
    double elb = (b - el).dotProduct(abDir);
    double fla = (a - fl).dotProduct(abDir);
    double flb = (b - fl).dotProduct(abDir);

    // test (must be the same than above)
    //    double cel = (el - c).dotProduct(abDir);
    //    double cfl = (fl - c).dotProduct(abDir);

    //    double gal = (al - g).dotProduct(efDir);
    //    double gbl = (bl - g).dotProduct(efDir);

    //    double ale = ge - gal;
    //    double alf = gf - gal;
    //    double ble = ge - gbl;
    //    double blf = gf - gbl;

    //    double ela = ca - cel;
    //    double fla = ca - cfl;
    //    double elb = cb - cel;
    //    double flb = cb - cfl;

    double ca = (a - c).dotProduct(abDir);
    double cb = (b - c).dotProduct(abDir);
    double ge = (e - g).dotProduct(efDir);
    double gf = (f - g).dotProduct(efDir);

    double m = 0;

    double beBle = be + ble;
    double bfBlf = bf + blf;
    double m1 = cb * std::log(bfBlf / beBle);
    if(!std::isnan(m1) && !std::isinf(m1))
        m += m1;

    double aeAle = ae + ale;
    double afAlf = af + alf;
    double m2 = ca * std::log(afAlf / aeAle);
    if(!std::isnan(m2) && !std::isinf(m2))
        m -= m2;

    double afFla = af + fla;
    double bfFlb = bf + flb;
    double m3 = gf * std::log(bfFlb / afFla);
    if(!std::isnan(m3) && !std::isinf(m3))
        m += m3;

    double aeEla = ae + ela;
    double beElb = be + elb;
    double m4 = ge * std::log(beElb / aeEla);
    if(!std::isnan(m4) && !std::isinf(m4))
        m -= m4;

    if(isParallel) {
        // [1] Eq. A-6
        m -= (be + af - bf - ae) * cosTheta;
    }
    else if(isOrthogonal) {
        // [1] Eq. A-11
        double cg = c.distanceTo(g);
        if(cg > precision) {
            double omega1 = std::atan(cb * gf / (cg * bf));
            double omega2 = std::atan(cb * ge / (cg * be));
            double omega3 = std::atan(ca * gf / (cg * af));
            double omega4 = std::atan(ca * ge / (cg * ae));
            if(std::isnan(omega1))
                omega1 = 0;
            if(std::isnan(omega2))
                omega2 = 0;
            if(std::isnan(omega3))
                omega3 = 0;
            if(std::isnan(omega4))
                omega4 = 0;

            double u = cg * (omega1 - omega2 - omega3 + omega4);
            m -= std::abs(u);
        }
    }
    else {
        // [1] Eq. A-3
        double cg = c.distanceTo(g);
        if(cg > precision) {
            double sinTheta = std::sqrt(1 - cosTheta*cosTheta);
            double tanTheta = sinTheta / cosTheta;

            double omega1 = std::atan(cg / (bf * tanTheta) + cb * gf * sinTheta / (cg * bf));
            double omega2 = std::atan(cg / (be * tanTheta) + cb * ge * sinTheta / (cg * be));
            double omega3 = std::atan(cg / (af * tanTheta) + ca * gf * sinTheta / (cg * af));
            double omega4 = std::atan(cg / (ae * tanTheta) + ca * ge * sinTheta / (cg * ae));
            if(std::isnan(omega1))
                omega1 = 0;
            if(std::isnan(omega2))
                omega2 = 0;
            if(std::isnan(omega3))
                omega3 = 0;
            if(std::isnan(omega4))
                omega4 = 0;

            double omega = omega1 - omega2 - omega3 + omega4;
            m -= cg * omega / sinTheta;
        }
    }
    return m;
}

void GroundingSystem::calculateSegments()
{
    m_segments.clear();
    m_segments.reserve(m_conductors.size());

    for(Conductor& conductor : m_conductors) {
        // Break conductor in two if it's on both layers.
        double z = -getFirstLayerDepth();
        if(m_numberOfLayers > 1 && ((z > conductor.p0.z() && z < conductor.p1.z()) || (z < conductor.p0.z() && z > conductor.p1.z()))) {
            double len = conductor.length();
            Vector3Dd dir = (conductor.p1 - conductor.p0).normalized();

            Conductor c1 = conductor;
            c1.p1 = c1.p0 + dir * (std::abs(z - conductor.p0.z()) * len / std::abs(conductor.p1.z() - conductor.p0.z()));
            calculateConductorSegments(c1);

            Conductor c2 = conductor;
            c2.p0 = c2.p1 - dir * (std::abs(z - conductor.p1.z()) * len / std::abs(conductor.p1.z() - conductor.p0.z()));
            calculateConductorSegments(c2);
        }
        else
            calculateConductorSegments(conductor);
    }
}

void GroundingSystem::calculateConductorSegments(const Conductor& conductor)
{
    int conductorIndex = m_conductors.indexOf(conductor);
    int n = std::ceil(conductor.length() / getSegmentMaximumLength());
    Vector3Dd dp = (conductor.p1 - conductor.p0) / (double)n;
    for(int i = 0; i < n; ++i) {
        ConductorSegment cs;
        cs.a = conductor.p0 + dp * i;
        cs.b = conductor.p0 + dp * (i + 1);
        cs.radius = conductor.getDiameter() / 2000.;
        cs.dir = (cs.b - cs.a).normalized();
        cs.length = cs.a.distanceTo(cs.b);
        cs.averageZ = (cs.a.z() + cs.b.z()) / 2.;
        cs.conductorIndex = conductorIndex;
        m_segments.push_back(cs);
    }
}

CablePtr GroundingSystem::getCableByDiameter(double diameter)
{
    CablePtr cable = g_database->getCableByDiameterAndType(CableCategory_ShieldWire, diameter, "Copperweld");

    if(!cable) // Priority to copper cables
        cable = g_database->getCableByDiameterAndType(CableCategory_ShieldWire, diameter, "Copper");

    if(!cable) // Priority to copperweld cables
        cable = g_database->getCableByDiameterAndType(CableCategory_ShieldWire, diameter, "Copperweld");

    if(!cable) // Priority to cable with same diameter
        cable = g_database->getCableByDiameter(CableCategory_ShieldWire, diameter);

    if(!cable) { // Any shield wire cable
        const QVector<CablePtr>& shieldWires = g_database->getCablesByCategory(CableCategory_ShieldWire);
        if(!shieldWires.empty())
            cable = shieldWires.first();
    }
    return cable;
}

QString GroundingSystem::getErrorLocation()
{
    return QObject::tr("\n\nAt:\n") + QObject::tr("Grounding system") + ": " + m_name;
}

QString Conductor::getCode() const
{
    if(cable)
        return cable->getCode();
    return "";
}

double Conductor::getDiameter() const
{
    if(cable) {
        double d =  cable->getDiameter();
        return cable->getDiameter();
    }
    return 1;
}

//////////////////////////////////////////////

bool GroundingSystem::calculateResistance(GroundingSystemPtr gs, QString& error)
{
    int i, j;
    if(gs->canMergeConductors(i, j)) {
        error += QObject::tr("Conductors %1 and %2 can be merged.\nA calculation error might happen if they are overlapping each other.\nIt's recommended to press 'Join' button on data input.").arg(i+1).arg(j+1);
        return false;
    }


    for(int i = 0; i < 10; ++i) { // 10 tries to calculate resistance
        try {
            if(gs->calculateResistance(error))
                return true;
        }
        catch(std::bad_alloc&) {
            double length = gs->getSegmentMaximumLength() * 2;
            error += QObject::tr("Out of memory. New maximum segment length is %1.").arg(length);
            gs->setSegmentMaximumLength(QString::number(length));
        }
    }
    return false;
}

void GroundingSystem::calculateSurfaceVoltage(GroundingSystemPtr gs)
{
    for(const Vector3Dd& point : gs->getSurfaceVoltagePoints())
        gs->getSurfaceVoltage(point);

    double meshVoltage = gs->getResistance() * gs->getInjectedCurrent();

    for(SurfaceVoltageProfile<Vector3Dd>& profile : gs->getSurfaceVoltageProfiles()) {
        Vector3Dd dir = (profile.pf - profile.pi).normalized();

        profile.maxTouchVoltage = 0;
        profile.maxStepVoltage = 0;

        double x0 = 0;
        double x1 = profile.pi.distanceTo(profile.pf);
        for(double x = x0; x <= x1; x += profile.precision) {
            Vector3Dd point = profile.pi + dir * x;
            double v = gs->getSurfaceVoltage(point);

            if(point.y() < 1 || point.y() > 13.3)
                continue;

            double touchVoltage = std::abs(meshVoltage - v);
            if(touchVoltage > profile.maxTouchVoltage) {
                profile.maxTouchVoltage = touchVoltage;
                profile.maxTouchVoltagePos = point;
            }
        }

        double stepSize = 1; // m
        x0 = -profile.stepSearchMargin;
        x1 = profile.pi.distanceTo(profile.pf) + profile.stepSearchMargin;
        for(double x = x0; x <= x1 - stepSize; x += profile.precision) {
            Vector3Dd point = profile.pi + dir * x;
            Vector3Dd pointStep = profile.pi + dir * (x + stepSize);

            double v = gs->getSurfaceVoltage(point);
            double vStep = gs->getSurfaceVoltage(pointStep);

            double stepVoltage = std::abs(vStep - v);
            if(stepVoltage > profile.maxStepVoltage) {
                profile.maxStepVoltage = stepVoltage;
                profile.maxStepVoltagePos1 = point;
                profile.maxStepVoltagePos2 = pointStep;
            }
        }
    }
}
