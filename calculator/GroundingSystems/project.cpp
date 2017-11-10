#include "project.h"
#include "luainterface.h"
#include "groundingsystem.h"
#include "groundingsystemreport.h"
#include <QSettings>
#include <QDebug>
#include <QDir>
#include <vector3d.h>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

Project *g_project = nullptr;

Project::Project()
{
    QDir prev = QCoreApplication::applicationDirPath();
    prev.cd("..");
    QSettings *settings = new QSettings(prev.path() + "/input.ftl", QSettings::IniFormat);

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

    if(error.isEmpty()) {
        QDir dir;
        dir.remove(prev.path() + "/results.ftl");
        generateReport(prev.path() + "/results.ftl");
    }
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

void Project::generateReport(const QString &fileName)
{
    GroundingSystemReport *gsReport = new GroundingSystemReport();
    gsReport->generateResources();

    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    for(int i = 0; i < g_database->getGroundingSystems().size(); i++) {
        GroundingSystemPtr gs = g_database->getGroundingSystems()[i];
        QString gsArray = "GroundingSystems." + QString::number(i+1) + ".";
        out << gsArray << "id = " << gs->getId() << "\n";
        out << gsArray << "name = " << gs->getName() << "\n";
        out << gsArray << "resistance = " << gs->getResistance() << "\n";
        out << gsArray << "injectedCurrent = " << gs->getInjectedCurrent() << "\n";
        out << gsArray << "voltage = " << gs->getResistance() * gs->getInjectedCurrent() << "\n";

        for(int j = 0; j < gs->getSurfaceVoltagePoints().size(); j++) {
            const Vector3Dd& point = gs->getSurfaceVoltagePoints()[j];
            QString pointsArray = gsArray + "Points." + QString::number(j+1) + ".";
            out << pointsArray << "x = " << point.x() << "\n";
            out << pointsArray << "y = " << point.y() << "\n";
            out << pointsArray << "surfaceVoltage = " << gs->calculateSurfaceVoltage(point) << "\n";
        }

        for(int j = 0; j < gs->getSurfaceVoltageProfiles().size(); j++) {
            SurfaceVoltageProfile<Vector3Dd> profile = gs->getSurfaceVoltageProfiles()[j];
            QString profileArray = gsArray + "Profiles." + QString::number(j+1) + ".";
            out << profileArray << "touch = " << profile.touch << "\n";
            out << profileArray << "touchX = " << profile.maxTouchVoltagePos.x() << "\n";
            out << profileArray << "touchY = " << profile.maxTouchVoltagePos.y() << "\n";
            out << profileArray << "maxTouchVoltage = " << profile.maxTouchVoltage << "\n";
            out << profileArray << "step = " << profile.step << "\n";
            out << profileArray << "stepX0 = " << profile.maxStepVoltagePos1.x() << "\n";
            out << profileArray << "stepY0 = " << profile.maxStepVoltagePos1.y() << "\n";
            out << profileArray << "stepX1 = " << profile.maxStepVoltagePos2.x() << "\n";
            out << profileArray << "stepY1 = " << profile.maxStepVoltagePos2.y() << "\n";
            out << profileArray << "maxStepVoltage = " << profile.maxStepVoltage << "\n";
            out << profileArray << "precision = " << profile.precision << "\n";

            double x1 = profile.pi.distanceTo(profile.pf); //CHECKHERE; CONFERIR COM PROGRAMA ANTIGO; ERRADO
            double precision = profile.precision;
            Vector3Dd dir = (profile.pf - profile.pi).normalized();

            int pointIndex = 0;
            for(double x = 0; x < x1; x+=precision) {
                QString profilePointsArray = profileArray + "Points." + QString::number(pointIndex++) + ".";
                Vector3Dd point = profile.pi + (dir * x);
                out << profilePointsArray << "x = " << point.x() << "\n";
                out << profilePointsArray << "y = " << point.y() << "\n";
                out << profilePointsArray << "voltage = " << gs->calculateSurfaceVoltage(point) << "\n";
            }

            QVector<double> weights = {50, 70};
            for(int j = 0; j < weights.size(); j++) {
                QString limitsArray = gsArray + "Limits." + QString::number(j+1) + ".";
                double weight = weights[j];
                out << limitsArray << "weight = " << weight << "\n";
                int faultClearCount = 1;
                for(double faultTime = 0.5; faultTime <= 3; faultTime += 0.5) {
                    QString faultTimesArray = limitsArray + "FaultTimes." + QString::number(faultClearCount++) + ".";
                    double touchPotential = gs->calculateTouchPotentialLimit2013(faultTime, weight);
                    double stepPotential = gs->calculateStepPotentialLimit2013(faultTime, weight);
                    out << faultTimesArray << "faultTime = " << faultTime << "\n";
                    out << faultTimesArray << "touchPotential = " << touchPotential << "\n";
                    out << faultTimesArray << "stepPotential = " << stepPotential << "\n";
                }
            }

        }
    }

    file.close();
}
