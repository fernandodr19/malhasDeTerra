#include <QCoreApplication>
#include <QDebug>
#include "project.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    g_project = new Project();
    qDebug() << "end";
    return a.exec();
}

