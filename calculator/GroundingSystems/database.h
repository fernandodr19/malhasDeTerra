#ifndef DATABASE_H
#define DATABASE_H

#include <QVector>
#include <cable.h>
#include <groundingsystem.h>

class Database
{
public:
    Database();

    void load(QSettings *settings);
    void save(QSettings *settings);

    CablePtr getCableById(int cableId);
    QVector<CablePtr> getCablesByCategory(CableCategory cableCategory);
    CablePtr getCableByDiameter(CableCategory category, double diameter);
    CablePtr getCableByDiameterAndType(CableCategory category, double diameter, const QString& type);

    QVector<GroundingSystemPtr> getGroundingSystems() { return m_groundingSystems; }
    QVector<CablePtr> getCables() { return m_cables; }

private:
    QVector<GroundingSystemPtr> m_groundingSystems;
    QVector<CablePtr> m_cables;
};

extern Database *g_database;

#endif // DATABASE_H
