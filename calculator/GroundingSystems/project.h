#ifndef PROJECT_H
#define PROJECT_H

#include <QVector>
#include "database.h"
#include "cable.h"
#include "groundingsystem.h"

class Project
{
public:
    Project();

    bool calculate(QString& error);

private:

};

extern Project *g_project;

#endif // PROJECT_H
