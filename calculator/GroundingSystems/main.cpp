#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include "groundingsystem.h"

void loadCables();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GroundingSystem *gs = new GroundingSystem();
    gs->load();

    loadCables();
    qDebug() << "done";
    return a.exec();
}

void loadCables()
{
    QString filename = "C:\\Users\\fdaros\\Desktop\\chevron - Adaptado.ftl";
    QSettings settings(filename, QSettings::IniFormat);

//    settings.beginGroup("TransmissionLines");
//    for(int i = 0; i < nCables; ++i) {
//        settings->setArrayIndex(i);
//        CablePtr c = std::make_shared<Cable>(shared_from_this());
//        c->load(settings);
//        m_cables.push_back(c);
//    }
//    settings.endGroup();

    settings.beginGroup("TransmissionLines");
    for(const QString& transmissionLineGroup : settings.childGroups()) {
        qDebug() << "tl";
        settings.endGroup();
    }
    settings.endGroup();
}
