#ifndef GroundingSystemReport_H
#define GroundingSystemReport_H

//#include "reportview.h"
#include <groundingsystem.h>
#include <vector3d.h>

class GroundingSystem;

class GroundingSystemReport
{
//    Q_OBJECT

public:
    GroundingSystemReport();

    void generateResources();

private:
    void saveSurfaceVoltagePlot(GroundingSystemPtr groundingSystem, const SurfaceVoltageProfile<Vector3Dd>& profile, const QString& filename);
    void saveTouchVoltagePlot(GroundingSystemPtr groundingSystem, const SurfaceVoltageProfile<Vector3Dd>& profile, const QString& filename);

};

#endif // GroundingSystemReport_H
