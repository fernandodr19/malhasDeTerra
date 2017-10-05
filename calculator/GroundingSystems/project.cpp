#include "project.h"
#include "luainterface.h"
#include "groundingsystem.h"
#include "groundingsystemreport.h"
#include <QSettings>
#include <QDebug>
#include <QDir>
#include <vector3d.h>

Project *g_project = nullptr;

Project::Project()
{
    QString filename = "C:\\Users\\fdaros\\Desktop\\chevron - Adaptado.ftl";
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    load(settings);

    g_database = new Database();
    g_database->load(settings);

    QString error;
    if(!calculate(error))
        qDebug() << "Error:" << error;


//    if(!QDir("assets").exists()) {
//        QDir prev = QDir::current();
//        prev.cd("..");
//        QDir::setCurrent(prev.path());
//        if(!QDir("assets").exists())
//            qDebug() << "Could not find assets";
//    }

//    g_lua.initialize();
//    error = g_lua.executeFile("assets/lua/init.lua").c_str();
//    if(!error.isEmpty())
//        qDebug() << error;

    generateReport();
}

void Project::load(QSettings *settings)
{
    settings->beginGroup("Project");
    m_transmitterId = settings->value("transmitterId").toUInt();
    m_designerId = settings->value("designerId", 1).toUInt();
    m_epcId = settings->value("epcId").toUInt();
    m_type = (ProjectType) settings->value("type").toInt();
    m_reference = settings->value("reference").toString();
    m_auctionNumber = settings->value("auctionNumber").toString();
    m_lotNumber = settings->value("lotNumber").toString();
    m_title = settings->value("title").toString();
    m_description = settings->value("description").toString();
    m_date = settings->value("date").toString();
    settings->endGroup();
}

bool Project::calculate(QString& error)
{
    for(GroundingSystemPtr gs : g_database->getGroundingSystems()) {
        if(!gs->calculateResistance(gs, error))
            return false;

        gs->calculateSurfaceVoltage(gs);
    }

    return true;
}

void Project::generateReport()
{
    GroundingSystemReport *gsReport = new GroundingSystemReport();
    gsReport->generateResources();

    for(GroundingSystemPtr gs : g_database->getGroundingSystems()) {
        qDebug() << "\nGeneral info";
        qDebug() << gs->getName();
        qDebug() << gs->getResistance();
        qDebug() << gs->getInjectedCurrent();
        qDebug() << gs->getResistance() * gs->getInjectedCurrent();//voltage
        qDebug() << "\nSurface points";
        for(const Vector3Dd& point : gs->getSurfaceVoltagePoints()) {
            qDebug() << point.x();
            qDebug() << point.y();
            qDebug() << gs->calculateSurfaceVoltage(point);
        }
        qDebug() << "\nSurface voltage profile";
        for(SurfaceVoltageProfile<Vector3Dd> profile : gs->getSurfaceVoltageProfiles()) {
            qDebug() << profile.maxTouchVoltage;
            qDebug() << "(" << profile.maxTouchVoltagePos.x() << "," << profile.maxTouchVoltagePos.y() << ")";
            qDebug() << profile.maxStepVoltage;
            qDebug() << "(" << profile.maxStepVoltagePos1.x() << "," << profile.maxStepVoltagePos1.y() << ") ; (" << profile.maxStepVoltagePos2.x() << "," << profile.maxStepVoltagePos2.y() << ")";

            qDebug() << "\nPoints";
            double x1 = profile.pi.distanceTo(profile.pf);
            double precision = profile.precision;
            Vector3Dd dir = (profile.pf - profile.pi).normalized();
            for(double x = 0; x < x1; x+=precision) {
                Vector3Dd point = profile.pi + (dir * x);
                double voltage = gs->calculateSurfaceVoltage(point);
                qDebug() << point.x();
                qDebug() << point.y();
                qDebug() << voltage;
            }

            qDebug() << "generate ('groundingsystem_surface_%d_%d.png', gs.id, profileCount)";
            qDebug() << "generate ('groundingsystem_touch_%d_%d.png', gs.id, profileCount)";
        }
        qDebug() << "\nLimits";
        QVector<double> weights = {50, 70};
        for(double weight : weights) {
            for(double faultTime = 0.5; faultTime <= 3; faultTime += 0.5) {
                double touchPotential = gs->calculateTouchPotentialLimit2013(faultTime, weight);
                double stepPotential = gs->calculateStepPotentialLimit2013(faultTime, weight);
                qDebug() << faultTime;
                qDebug() << touchPotential;
                qDebug() << stepPotential;
                qDebug() << "";
            }
        }

    }
}
