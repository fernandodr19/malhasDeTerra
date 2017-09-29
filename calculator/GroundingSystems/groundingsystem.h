#ifndef GROUNDINGSYSTEM_H
#define GROUNDINGSYSTEM_H

#include <QVector>
#include <QMap>
#include <const.h>
#include <tmath.h>
#include <vector3d.h>

class QSettings;
class CableType;

struct ConductorSegment
{
    Vector3Dd a;
    Vector3Dd b;
    Vector3Dd dir;
    double averageZ;
    double radius;
    double length;
    double current1V;
    int conductorIndex;

    bool operator==(const ConductorSegment& c) const {
        return a == c.a && b == c.b;
    }
};

struct Conductor
{
    Conductor() {}
    Conductor(const Vector3Dd& v0, const Vector3Dd& v1, const CableTypePtr& c) :
        p0(v0), p1(v1), cable(c)
    {
    }

    double length() const {
        return p0.distanceTo(p1);
    }

    QString getCode() const;
    double getDiameter() const;

    Vector3Dd p0;
    Vector3Dd p1;
    CableTypePtr cable;

    bool operator==(const Conductor& c) const {
        return p0 == c.p0 && p1 == c.p1 && cable == c.cable;
    }
};

template<class T>
class SurfaceVoltageProfile
{
public:
    T pi;
    T pf;
    double precision = 1;
    bool touch = true;
    bool step = true;
    double stepSearchMargin = 5;

    // Cache
    double maxTouchVoltage = 0;
    T maxTouchVoltagePos;
    double maxStepVoltage = 0;
    T maxStepVoltagePos1;
    T maxStepVoltagePos2;
};

class GroundingSystem
{
public:
    GroundingSystem();

    void load();
    void save();

    bool validate(QString& error);
    bool calculate(QString& error);

    void addConductor(const Vector3Dd& pos1, const Vector3Dd& pos2, const CableTypePtr& cable);
    Conductor& getConductor(int index) { return m_conductors[index]; }
    QVector<Conductor>& getConductors() { return m_conductors; }
    void clearConductors() { m_conductors.clear(); }
    bool canMergeConductors(int& i, int& j);
    void mergeConductors();

    QVector<Vector3Dd>& getSurfaceVoltagePoints() { return m_surfaceVoltagePoints; }
    QVector<SurfaceVoltageProfile<Vector3Dd>>& getSurfaceVoltageProfiles() { return m_surfaceVoltageProfiles; }

    void setName(const QString& name) { m_name = name; }
    void setNumberOfLayers(int numberOfLayers) { m_numberOfLayers = numberOfLayers; }
    void setSegmentMaximumLength(const QString& segmentMaximumLength) { m_segmentMaximumLength = segmentMaximumLength; }
    void setCrushedStoneLayerDepth(const QString& depth) { m_crushedStoneLayerDepth = depth; }
    void setCrushedStoneLayerResistivity(const QString& rho) { m_crushedStoneLayerResistivity = rho; }
    void setFirstLayerDepth(const QString& depth) { m_firstLayerDepth = depth; }
    void setFirstLayerResistivity(const QString& rho) { m_firstLayerResistivity = rho; }
    void setSecondLayerResistivity(const QString& rho) { m_secondLayerResistivity = rho; }
    void setInjectedCurrent(const QString& injectedCurrent) { m_injectedCurrent = injectedCurrent; }
    void setIgnored(bool ignored) { m_ignored = ignored; }

    QString getName() { return m_name; }
    int getNumberOfLayers() { return m_numberOfLayers; }
    double getSegmentMaximumLength() { return m_segmentMaximumLength.toDouble(); }
    QString getSegmentMaximumLengthText() { return m_segmentMaximumLength; }
    double getCrushedStoneLayerDepth() { return m_crushedStoneLayerDepth.toDouble(); }
    QString getCrushedStoneLayerDepthText() { return m_crushedStoneLayerDepth; }
    double getCrushedStoneLayerResistivity() { return m_crushedStoneLayerResistivity.toDouble(); }
    QString getCrushedStoneLayerResistivityText() { return m_crushedStoneLayerResistivity; }
    double getFirstLayerDepth() { return m_firstLayerDepth.toDouble(); }
    QString getFirstLayerDepthText() { return m_firstLayerDepth; }
    double getFirstLayerResistivity() { return m_firstLayerResistivity.toDouble(); }
    QString getFirstLayerResistivityText() { return m_firstLayerResistivity; }
    double getSecondLayerResistivity() { return m_secondLayerResistivity.toDouble(); }
    QString getSecondLayerResistivityText() { return m_secondLayerResistivity; }
    double getInjectedCurrent() { return m_injectedCurrent.toDouble(); }
    QString getInjectedCurrentText() { return m_injectedCurrent; }
    QString getDxfFile() { return m_dxfFile; }

    bool calculateResistance(QString& error);
    double calculateSurfaceVoltage(const Vector3Dd& p);
    double calculateTouchPotentialLimit1986(double faultTime, int weight);
    double calculateStepPotentialLimit1986(double faultTime, int weight);
    double calculateTouchPotentialLimit2013(double faultTime, int weight);
    double calculateStepPotentialLimit2013(double faultTime, int weight);
    bool isConnected(int& disconnectedSegment);

    // Cache
    double getResistance() { return m_resistance; }
    double getSurfaceVoltage(const Vector3Dd& p);
    bool isIgnored() { return m_ignored; }

    static double calculateNumericalM(const ConductorSegment& c1, const ConductorSegment& c2);
    static double calculateM(const ConductorSegment& c1, const ConductorSegment& c2);

private:
    double calculateSegmentVoltage(const ConductorSegment& cs, const Vector3Dd& p);
    bool calculateMutualResistance(const ConductorSegment& c1, const ConductorSegment& cj, double& r, QString& error);
    void calculateSegments();
    void calculateConductorSegments(const Conductor& conductor);
    CableTypePtr getCableByDiameter(double diameter);
    QString getErrorLocation();

    QString m_name;
    int m_numberOfLayers = 1;
    QString m_segmentMaximumLength; // m
    QString m_crushedStoneLayerDepth; // m
    QString m_crushedStoneLayerResistivity; // ohm.m
    QString m_firstLayerDepth; // m
    QString m_firstLayerResistivity; // ohm.m
    QString m_secondLayerResistivity; // ohm.m
    QString m_injectedCurrent; // A
    QString m_dxfFile;

    QVector<Conductor> m_conductors;
    QVector<Vector3Dd> m_surfaceVoltagePoints;
    QVector<SurfaceVoltageProfile<Vector3Dd>> m_surfaceVoltageProfiles;

    // Cache
    double m_k; // reflection factor
    QVector<ConductorSegment> m_segments;
    double m_resistance; // ohm
    QMap<Vector3Dd, double> m_surfaceVoltages;
    bool m_ignored = false;
};

#endif // GROUNDINGSYSTEM_H
