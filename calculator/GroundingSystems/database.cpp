#include "database.h"
#include <QSettings>
#include <QDebug>
#include "cable.h"
#include "groundingsystem.h"
#include "const.h"

Database *g_database = nullptr;

Database::Database()
{

}

void Database::load(QSettings *settings)
{
//    settings->beginGroup("GroundingSystems");
//    for(const QString& gsGroup : settings->childGroups()) {
//        settings->beginGroup(gsGroup);
//        GroundingSystemPtr gs = std::make_shared<GroundingSystem>();
//        gs->load(settings);
//        m_groundingSystems.push_back(gs);
//        settings->endGroup();
//    }
//    settings->endGroup();

    settings->beginGroup("Cables");
    for(const QString& cableGroup : settings->childGroups()) {
        settings->beginGroup(cableGroup);
        CablePtr cable = std::make_shared<Cable>();
        cable->load(settings);
        m_cables.push_back(cable);
        settings->endGroup();
    }
    settings->endGroup();

    settings->beginGroup("GroundingSystems");
    for(const QString& gsGroup : settings->childGroups()) {
        settings->beginGroup(gsGroup);
        GroundingSystemPtr gs = std::make_shared<GroundingSystem>();
        gs->setId(settings->value("id").toInt());
        gs->setName(settings->value("name").toString());
        gs->setSegmentMaximumLength(settings->value("conductorsMaxLength").toString());
        gs->setNumberOfLayers(settings->value("nLayers").toInt());
        gs->setFirstLayerDepth(settings->value("firstLayerDepth").toString());
        gs->setFirstLayerResistivity(settings->value("firstLayerResistivity").toString());
        gs->setSecondLayerResistivity(settings->value("secondLayerResistivity").toString());
        gs->setCrushedStoneLayerDepth(settings->value("crushedStoneLayerDepth").toString());
        gs->setCrushedStoneLayerResistivity(settings->value("crushedStoneLayerResistivity").toString());
        gs->setInjectedCurrent(settings->value("injectedCurrent").toString());
        m_groundingSystems.push_back(gs);
        settings->endGroup();
    }
    settings->endGroup();

    settings->beginGroup("Conductors");
    for(const QString& conductorGroup : settings->childGroups()) {
        settings->beginGroup(conductorGroup);
        Conductor conductor;
        int gsId = settings->value("gsId").toInt();
        conductor.p0.setX(settings->value("x1").toDouble());
        conductor.p0.setY(settings->value("y1").toDouble());
        conductor.p0.setZ(settings->value("z1").toDouble());
        conductor.p1.setX(settings->value("x2").toDouble());
        conductor.p1.setY(settings->value("y2").toDouble());
        conductor.p1.setZ(settings->value("z2").toDouble());

        int cableId = settings->value("cableId").toInt();
        conductor.cable = g_database->getCableById(cableId);

        if(!conductor.cable) {
            const QVector<CablePtr>& shieldWires = g_database->getCablesByCategory(CableCategory_ShieldWire);
            if(!shieldWires.empty())
                conductor.cable = shieldWires.first();
        }

        if(GroundingSystemPtr gs = getGroundSystemById(gsId))
            gs->getConductors().push_back(conductor);
        settings->endGroup();
    }
    settings->endGroup();

    settings->beginGroup("Points");
    for(const QString& group : settings->childGroups()) {
        settings->beginGroup(group);
        Vector3Dd point;
        int gsId = settings->value("gsId").toInt();
        point.setX(settings->value("x").toDouble());
        point.setY(settings->value("y").toDouble());
        if(GroundingSystemPtr gs = getGroundSystemById(gsId))
            gs->getSurfaceVoltagePoints().push_back(point);
        settings->endGroup();
    }
    settings->endGroup();

    settings->beginGroup("Profiles");
    for(const QString& group : settings->childGroups()) {
        settings->beginGroup(group);
        SurfaceVoltageProfile<Vector3Dd> profile;
        int gsId = settings->value("gsId").toInt();
        profile.pi.setX(settings->value("x0").toDouble());
        profile.pi.setY(settings->value("y0").toDouble());
        profile.pf.setX(settings->value("x1").toDouble());
        profile.pf.setY(settings->value("y1").toDouble());
        profile.precision = settings->value("precision", 1).toDouble();
        profile.touch = settings->value("touch", true).toBool();
        profile.step = settings->value("step", true).toBool();
        if(GroundingSystemPtr gs = getGroundSystemById(gsId))
            gs->getSurfaceVoltageProfiles().push_back(profile);
        settings->endGroup();
    }
    settings->endGroup();
}

void Database::save(QSettings *settings)
{

}

CablePtr Database::getCableById(int cableId)
{
    for(const CablePtr& cable : m_cables)
        if(cable->getId() == cableId)
            return cable;
    return nullptr;
}

GroundingSystemPtr Database::getGroundSystemById(int id)
{
    for(GroundingSystemPtr gs : m_groundingSystems) {
        if(gs->getId() == id)
            return gs;
    }
    return nullptr;
}

QVector<CablePtr> Database::getCablesByCategory(CableCategory cableCategory)
{
    QVector<CablePtr> cables;
    for(const CablePtr& cable : m_cables)
        if(cable->getCategory() == cableCategory)
            cables.push_back(cable);
    return cables;
}

CablePtr Database::getCableByDiameter(CableCategory category, double diameter)
{
    for(const CablePtr& cable : m_cables)
        if(cable->getCategory() == category && std::abs(cable->getDiameter() - diameter) <= 1E-03)
            return cable;
    return nullptr;
}

CablePtr Database::getCableByDiameterAndType(CableCategory category, double diameter, const QString& type)
{
    for(const CablePtr& cable : m_cables)
        if(cable->getCategory() == category && std::abs(cable->getDiameter() - diameter) <= 1E-03 && cable->getType() == type)
            return cable;
    return nullptr;
}
