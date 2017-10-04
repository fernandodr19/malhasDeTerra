#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>
#include <QVector>
#include "vector3d.h"
#include <Eigen/Core>
#include "polynomial.h"

class Settings
{
public:
    static QString toLuaTable(QSettings *settings);
    static bool isEqual(QSettings *a, QSettings *b);

    template<class T>
    static void saveComplex(QSettings *settings, const QString& group, const std::complex<T>& c) {
        settings->beginGroup(group);
        settings->setValue("real", c.real());
        settings->setValue("imag", c.imag());
        settings->endGroup();
    }

    template<class T>
    static void saveVector3D(QSettings *settings, const QString& group, const Vector3D<T>& vector) {
        settings->beginGroup(group);
        settings->setValue("x", vector.x());
        settings->setValue("y", vector.y());
        settings->setValue("z", vector.z());
        settings->endGroup();
    }

    template<class T>
    static void saveVector(QSettings *settings, const QString& group, const QVector<T>& vector) {
        settings->beginGroup(group);
        settings->setValue("_isArray", true);
        int size = vector.size();
        int digits = (int)std::log10(size) + 1;
        for(int i = 0; i < vector.size(); ++i) {
            settings->beginGroup(QString("Element%1").arg(i, digits, 10, QChar('0')));
            settings->setValue(QString::number(i), vector[i]);
            settings->endGroup();
        }
        settings->endGroup();
    }

    template<class T>
    static void loadVector(QSettings *settings, const QString& group, QVector<T>& vector)
    {
        settings->beginGroup(group);
        int elementNumber = 0;
        for(const QString& key : settings->childGroups()) {
            settings->beginGroup(key);
            QVariant settingValue = settings->value(QString::number(elementNumber));
            vector.push_back(settingValue.value<T>());
            settings->endGroup();
            elementNumber++;
        }
        settings->endGroup();
    }

    static void saveVector(QSettings *settings, const QString& group, const QVector<Vector3Dd>& vector);

    template<class R>
    static void saveVector(QSettings *settings, const QString& group, const QVector<std::complex<R>>& vector) {
        settings->beginGroup(group);
        settings->setValue("_isArray", true);
        int size = vector.size();
        int digits = (int)std::log10(size) + 1;
        for(int i = 0; i < vector.size(); ++i)
            saveComplex(settings, QString("Element%1").arg(i, digits, 10, QChar('0')), vector[i]);
        settings->endGroup();
    }

    template<class R>
    static void loadMap(QSettings *settings, const QString& group, QMap<QString, R>& map) {
        settings->beginGroup(group);
        for(const QString& key : settings->childGroups()) {
            settings->beginGroup(key);
            map[key] = settings->value(key).value<R>();
            settings->endGroup();
        }
        settings->endGroup();
    }

    template<class R>
    static void saveMap(QSettings *settings, const QString& group, const QMap<double, R>& map) {
        if(map.isEmpty())
            return;

        settings->beginGroup(group);
        settings->setValue("_isKeyArray", true);
        for(auto it = map.begin(); it != map.end(); ++it) {
            settings->beginGroup(QString::number(it.key()));
            settings->setValue(QString::number(it.key()), it.value());
            settings->endGroup();
        }
        settings->endGroup();
    }

    template<class R>
    static void saveMap(QSettings *settings, const QString& group, const QMap<QString, R>& map) {
        if(map.isEmpty())
            return;

        settings->beginGroup(group);
        settings->setValue("_isKeyArray", true);
        for(auto it = map.begin(); it != map.end(); ++it) {
            settings->beginGroup(it.key());
            settings->setValue(it.key(), it.value());
            settings->endGroup();
        }
        settings->endGroup();
    }

    template<class R>
    static void saveMap(QSettings *settings, const QString& group, const QMap<QString, std::complex<R>>& map) {
        settings->beginGroup(group);
        settings->setValue("_isKeyArray", true);
        for(auto it = map.begin(); it != map.end(); ++it)
            saveComplex(settings, it.key(), it.value());
        settings->endGroup();
    }

    template<class R>
    static void saveMap(QSettings *settings, const QString& group, const QMap<QString, QVector<std::complex<R>>>& map) {
        settings->beginGroup(group);
        settings->setValue("_isKeyArray", true);
        for(auto it = map.begin(); it != map.end(); ++it)
            saveVector(settings, it.key(), it.value());
        settings->endGroup();
    }

    static void saveMatrix(QSettings *settings, const QString& group, const Eigen::MatrixXd& matrix);
    static void saveMatrix(QSettings *settings, const QString& group, const Eigen::MatrixXcd& matrix);

    static void savePolynomial(QSettings *settings, const QString& group, Polynomial& polynomial);
    static void loadPolynomial(QSettings *settings, const QString& group, Polynomial& polynomial);

    static bool renameGroup(QSettings *settings, const QString& group, const QString& newGroup);
};

#endif // SETTINGS_H
