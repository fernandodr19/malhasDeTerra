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
    GroundingSystemPtr getGroundSystemById(int id);

    const QVector<GroundingSystemPtr>& getGroundingSystems() { return m_groundingSystems; }
    const QVector<CablePtr>& getCables() { return m_cables; }

    //LUA
//    GroundingSystemPtr addGroundingSystem();
    void addGroundingSystem(const GroundingSystemPtr& groundingSystem) { m_groundingSystems.push_back(groundingSystem); }
//    bool removeGroundingSystem(int index, QString& error);
    void removeGroundingSystems() { m_groundingSystems.clear(); }
    GroundingSystemPtr getGroundingSystem(int index) { return m_groundingSystems.value(index); }
    int getGroundingSystemIndex(const GroundingSystemPtr& groundingSystem) { return m_groundingSystems.indexOf(groundingSystem); }

private:
    QVector<GroundingSystemPtr> m_groundingSystems;
    QVector<CablePtr> m_cables;
};

extern Database *g_database;

#endif // DATABASE_H
