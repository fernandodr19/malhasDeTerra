#include <QApplication>
#include <QDebug>
#include "project.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "start";
    g_project = new Project();
    qDebug() << "end";
    return a.exec();
}
