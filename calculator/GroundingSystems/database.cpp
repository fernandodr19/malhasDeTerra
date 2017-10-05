#include "database.h"
#include <QSettings>
#include "cable.h"
#include "groundingsystem.h"
#include <QDebug>
#include "const.h"
#include <memory>

Database *g_database = nullptr;

Database::Database()
{

}

void Database::load(QSettings *settings)
{
    //CABLES
    settings->beginGroup("Cables");
    for(const QString& cableGroup : settings->childGroups()) {
        settings->beginGroup(cableGroup);
        CablePtr cable = std::make_shared<Cable>();
        cable->load(settings);
        m_cables.push_back(cable);
        qDebug() << cable->getId();
        settings->endGroup();
    }
    settings->endGroup();

    //GS
    settings->beginGroup("GroundingSystems");
    for(const QString& gsGroup : settings->childGroups()) {
        settings->beginGroup(gsGroup);
        GroundingSystemPtr gs = std::make_shared<GroundingSystem>();
        gs->load(settings);
        m_groundingSystems.push_back(gs);
        qDebug() << gs->getName();
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
