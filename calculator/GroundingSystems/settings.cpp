#include "settings.h"
#include <functional>
#include <QDebug>

QString Settings::toLuaTable(QSettings *settings)
{
    QString table;
    std::function<QString(QString,bool)> luaValue = [](QString value, bool key) {
        if(value == "true" || value == "false")
            return value;

        bool ok;
        value.toInt(&ok);
        if(ok)
            return value;

        double d = value.toDouble(&ok);
        if(ok) {
            if(std::isnan(d))
                return QString("0/0");
            else if(d == std::numeric_limits<double>::infinity())
                return QString("math.huge");
            else if(d == -std::numeric_limits<double>::infinity())
                return QString("-math.huge");
        }

        if(key)
            return QString("'%1'").arg(value.replace("'", "\\'"));
        else
            return QString("[[%1]]").arg(value);
    };

    std::function<void(QString,bool)> writeGroup = [&](const QString& group, bool isArray) {
        if(isArray)
            table += QString("%1 = ").arg(group);
        else {
            QString groupName = group.left(1).toLower() + group.mid(1);
            table += QString("['%1'] = ").arg(groupName.replace("'", "\\'"));
        }

        if(settings->value("_isArray").toBool()) {
            table += "{";
            int index = 0;
            const QStringList& childGroups = settings->childGroups();
            for(const QString& arrayGroup : childGroups) {
                settings->beginGroup(arrayGroup);
                writeGroup(QString("[%1]").arg(++index), true);
                settings->endGroup();
            }

            if(childGroups.isEmpty()) {
                int k = 0;
                for(const QString& childKey : settings->childKeys()) {
                    QString value = settings->value(childKey).toString();
                    if(!childKey.startsWith("_"))
                        table += QString("[%1] = %2,").arg(++k).arg(luaValue(value, false));
                }
            }
            table += "},";
        }
        else if(settings->value("_isKeyArray").toBool()) {
            table += "{";
            const QStringList& childGroups = settings->childGroups();
            for(const QString& arrayGroup : childGroups) {
                settings->beginGroup(arrayGroup);
                writeGroup(QString("[%1]").arg(luaValue(arrayGroup, true)), true);
                settings->endGroup();
            }
            table += "},";
        }
        else {
            const QStringList& childKeys = settings->childKeys();
            const QStringList& childGroups = settings->childGroups();
            if(childKeys.size() == 1 && childGroups.size() == 0) {
                QString value = settings->value(childKeys.first()).toString();
                table += QString("%1,").arg(luaValue(value, false));
            }
            else {
                table += "{";
                for(const QString& key : childKeys) {
                    if(!key.startsWith("_")) {
                        QString value = settings->value(key).toString();
                        table += QString("[%1] = %2,").arg(luaValue(key, true)).arg(luaValue(value, false));
                    }
                }

                for(const QString& childGroup : childGroups) {
                    settings->beginGroup(childGroup);
                    writeGroup(childGroup, false);
                    settings->endGroup();
                }
                table += "},";
            }
        }
        table += "\n";
    };

    for(const QString& childGroup : settings->childGroups()) {
        settings->beginGroup(childGroup);
        writeGroup(childGroup, false);
        settings->endGroup();
    }

    return table;
}

bool Settings::isEqual(QSettings *a, QSettings *b)
{
    for(const QString& key : a->allKeys()) {
        if(a->value(key) != b->value(key)) {
            //qDebug() << key << a->value(key) << b->value(key);
            return false;
        }
    }
    for(const QString& key : b->allKeys()) {
        if(a->value(key) != b->value(key)) {
            //qDebug() << key << a->value(key) << b->value(key);
            return false;
        }
    }
    return true;
}

void Settings::saveVector(QSettings *settings, const QString& group, const QVector<Vector3Dd>& vector)
{
    settings->beginGroup(group);
    settings->setValue("_isArray", true);
    int size = vector.size();
    int digits = (int)std::log10(size) + 1;
    for(int i = 0; i < vector.size(); ++i)
        saveVector3D(settings, QString("Element%1").arg(i, digits, 10, QChar('0')), vector[i]);
    settings->endGroup();
}

void Settings::saveMatrix(QSettings *settings, const QString& group, const Eigen::MatrixXd& matrix)
{
    settings->beginGroup(group);
    settings->setValue("_isArray", true);
    int rDigits = (int)std::log10(matrix.rows()) + 1;
    for(int r = 0; r < matrix.rows(); ++r) {
        settings->beginGroup(QString("%1").arg(r, rDigits, 10, QChar('0')));
        settings->setValue("_isArray", true);
        int cDigits = (int)std::log10(matrix.cols()) + 1;
        for(int c = 0; c < matrix.cols(); ++c)
            settings->setValue(QString("%1").arg(c, cDigits, 10, QChar('0')), matrix(r, c));
        settings->endGroup();
    }
    settings->endGroup();
}

void Settings::saveMatrix(QSettings *settings, const QString& group, const Eigen::MatrixXcd& matrix)
{
    settings->beginGroup(group);
    settings->setValue("_isArray", true);
    int rDigits = (int)std::log10(matrix.rows()) + 1;
    for(int r = 0; r < matrix.rows(); ++r) {
        settings->beginGroup(QString("%1").arg(r, rDigits, 10, QChar('0')));
        settings->setValue("_isArray", true);
        int cDigits = (int)std::log10(matrix.cols()) + 1;
        for(int c = 0; c < matrix.cols(); ++c)
            saveComplex(settings, QString("%1").arg(c, cDigits, 10, QChar('0')), matrix(r, c));
        settings->endGroup();
    }
    settings->endGroup();
}

void Settings::savePolynomial(QSettings *settings, const QString& group, Polynomial& polynomial)
{
    int size = polynomial.getCoeffSize();
    int digits = (int)std::log10(size) + 1;

    settings->beginGroup(group);
    settings->setValue("coeffSize", size);
    settings->beginGroup("coeffs");
    settings->setValue("_isArray", true);
    for(int i = 0; i < size; ++i)
        settings->setValue(QString("%1").arg(i, digits, 10, QChar('0')), polynomial.getCoeff(i));

    settings->endGroup();
    settings->endGroup();
}

void Settings::loadPolynomial(QSettings *settings, const QString& group, Polynomial& polynomial)
{
    settings->beginGroup(group);
    polynomial.setCoeffSize(settings->value("coeffSize").toInt());

    settings->beginGroup("coeffs");
    int count = 0;
    for(const QString& key : settings->childKeys()) {
        polynomial.setCoeff(count, settings->value(key).toString());
        count++;
    }
    settings->endGroup();
    settings->endGroup();
}

bool Settings::renameGroup(QSettings *settings, const QString& group, const QString& newGroup)
{
    if(group == newGroup)
        return true;
    if(settings->childGroups().contains(newGroup))
        return false;

    settings->beginGroup(group);
    QStringList keys = settings->allKeys();
    QMap<QString, QVariant> values;
    for(const QString& key : keys)
        values[key] = settings->value(key);
    settings->remove("");
    settings->endGroup();

    settings->beginGroup(newGroup);
    for(const QString& key : keys)
        settings->setValue(key, values[key]);
    settings->endGroup();
    return true;
}
