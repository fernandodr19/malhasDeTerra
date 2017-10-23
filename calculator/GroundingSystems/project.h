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

    void load(QSettings *settings);

    bool calculate(QString& error);

    void generateReport(const QString& fileName);

private:
    uint m_transmitterId;
    uint m_designerId;
    uint m_epcId;
    ProjectType m_type;
    QString m_reference;
    QString m_auctionNumber;
    QString m_lotNumber;
    QString m_title;
    QString m_description;
    QString m_date;
};

extern Project *g_project;

#endif // PROJECT_H
