#include <QCoreApplication>
#include <QDebug>
#include "groundingsystem.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GroundingSystem *gs = new GroundingSystem();
    gs->load();

    qDebug() << "done";
    return a.exec();
}
