#include "project.h"
#include <QSettings>
#include <QDebug>

Project *g_project = nullptr;

Project::Project()
{
    QString filename = "C:\\Users\\fdaros\\Desktop\\chevron - Adaptado.ftl";
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);
    g_database = new Database();
    g_database->load(settings);

    QString error;
    if(!calculate(error))
        qDebug() << "Error:" << error;
}

bool Project::calculate(QString& error)
{
    for(GroundingSystemPtr gs : g_database->getGroundingSystems()) {
        if(!gs->calculateResistance(gs, error))
            return false;

        gs->calculateSurfaceVoltage(gs);
        qDebug() << "Points" <<gs->getSurfaceVoltagePoints().size();
    }

    return true;
}
