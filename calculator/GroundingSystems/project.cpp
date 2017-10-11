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
//    QString filename = "C:\\Users\\fdaros\\Desktop\\chevron - Adaptado.ftl";
    QDir prev = QDir::current();
    prev.cd("..");
    QSettings *settings = new QSettings(prev.path() + "/chevron.ftl", QSettings::IniFormat);

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

    QDir dir;
    dir.remove(prev.path() + "/results.ftl");
    QSettings *saveSettings = new QSettings(prev.path() + "/results.ftl", QSettings::IniFormat);
    generateReport(saveSettings);
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

void Project::generateReport(QSettings *settings)
{
    GroundingSystemReport *gsReport = new GroundingSystemReport();
    gsReport->generateResources();

    settings->beginWriteArray("GroundingSystems");
    settings->setValue("_isKeyArray", true);
    for(int i = 0; i < g_database->getGroundingSystems().size(); i++) {
        GroundingSystemPtr gs = g_database->getGroundingSystems()[i];
        settings->setArrayIndex(i);
        settings->setValue("name", gs->getName());
        settings->setValue("resistance", gs->getResistance());
        settings->setValue("injectedCurrent", gs->getInjectedCurrent());
        settings->setValue("voltage", gs->getResistance() * gs->getInjectedCurrent());

        settings->beginWriteArray("SurfacePoints");
        settings->setValue("_isKeyArray", true);
        for(int j = 0; j < gs->getSurfaceVoltagePoints().size(); j++) {
            const Vector3Dd& point = gs->getSurfaceVoltagePoints()[j];
            settings->setArrayIndex(j);
            settings->setValue("x", point.x());
            settings->setValue("y", point.y());
            settings->setValue("surfaceVoltage", gs->calculateSurfaceVoltage(point));
        }
        settings->endArray();

        settings->beginWriteArray("SurfaceProfiles");
        settings->setValue("_isKeyArray", true);
        for(int j = 0; j < gs->getSurfaceVoltageProfiles().size(); j++) {
            SurfaceVoltageProfile<Vector3Dd> profile = gs->getSurfaceVoltageProfiles()[j];
            settings->setArrayIndex(j);
            settings->setValue("touchX", profile.maxTouchVoltagePos.x());
            settings->setValue("touchY", profile.maxTouchVoltagePos.y());
            settings->setValue("maxTouchVoltage", profile.maxTouchVoltage);
            settings->setValue("stepX0", profile.maxStepVoltagePos1.x());
            settings->setValue("stepY0", profile.maxStepVoltagePos1.y());
            settings->setValue("stepX1", profile.maxStepVoltagePos2.x());
            settings->setValue("stepY1", profile.maxStepVoltagePos2.y());
            settings->setValue("maxStepVoltage", profile.maxStepVoltage);
            settings->setValue("precision", profile.precision);

            double x1 = profile.pi.distanceTo(profile.pf);
            double precision = profile.precision;
            Vector3Dd dir = (profile.pf - profile.pi).normalized();

            settings->beginWriteArray("Points");
            settings->setValue("_isKeyArray", true);
            int pointIndex = 0;
            for(double x = 0; x < x1; x+=precision) {
                settings->setArrayIndex(pointIndex++);
                Vector3Dd point = profile.pi + (dir * x);
                settings->setValue("x", point.x());
                settings->setValue("y", point.y());
                settings->setValue("voltage", gs->calculateSurfaceVoltage(point));
            }
            settings->endArray();
        }
        settings->endArray();

        QVector<double> weights = {50, 70};
        settings->beginWriteArray("Limits");
        for(int j = 0; j < weights.size(); j++) {
            double weight = weights[j];
            settings->setArrayIndex(weight-1);
            for(double faultTime = 0.5; faultTime <= 3; faultTime += 0.5) {
                double touchPotential = gs->calculateTouchPotentialLimit2013(faultTime, weight);
                double stepPotential = gs->calculateStepPotentialLimit2013(faultTime, weight);

                settings->setValue("faultTime", faultTime);
                settings->setValue("touchPotential", touchPotential);
                settings->setValue("stepPotential", stepPotential);
            }
        }
        settings->endArray();
    }
    settings->endArray();
}
