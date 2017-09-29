#ifndef CABLE_H
#define CABLE_H

#include <QString>
#include <QVector>
#include <QMap>
#include <const.h>
//#include <catenary.h>

class QSettings;
class Project;
//struct Catenary;

class CableSag
{
public:
    bool validate(QString& error, int i);

    void setType(CableConditionType type) { m_type = type; }
    void setSag(const QString& sag) { m_sag = sag; }
    void setSpan(const QString& span) { m_span = span; }

    CableConditionType getType() const { return m_type; }
    QString getSag() const { return m_sag; }
    QString getSpan() const { return m_span; }

private:
    CableConditionType m_type = CableConditionType_Other;
    QString m_sag;
    QString m_span;
};

struct CableConditionSag
{
    double initialHorizontalTension; // kgf
    double initialTangentialTension; // kgf
    double initialSag; // m
    double finalHorizontalTension; // kgf
    double finalTangentialTension; // kgf
    double finalSag; // m
};

//struct CableConditionResult
//{
//    double horizontalTension;
//    double cableSag;
//    double chainSag;
//    double totalSag;

//    double catenaryMaximumSagX;
//    double catenaryMaximumSagY;
//    double catenaryLowerPointX;
//    double catenaryLowerPointY;

//    InsulatorStringTypePtr cadL;
//    InsulatorStringTypePtr cadR;
//    Catenary cat;
//};

class CableCondition
{
public:
    explicit CableCondition();

    void load(QSettings *settings);
    void save(QSettings *settings, bool saveTemporary);

    bool validate(Project *project, QString& error, bool governmentCondition, int i);

    void setType(CableConditionType type) { m_type = type; }
    void setName(const QString& name) { m_name = name; }
    void setTemperature(const QString& temperature) { m_temperature = temperature; }
    void setWindPressure(const QString& windPressure) { m_windPressure = windPressure; }
    void setUltimateLoading(const QString& loading) { m_ultimateLoading = loading; }
    void setLoadingCondition(LoadingCondition condition) { m_loadingCondition = condition; }

    CableConditionType getType() { return m_type; }
    QString getName() { return m_name; }
    QString getTemperature() { return m_temperature; }
    QString getWindPressure() { return m_windPressure; }
    QString getUltimateLoading() { return m_ultimateLoading; }
//    const QMap<QString, CableConditionResult>& getSagResults() { return m_sagResults; }
    QMap<QString, CableConditionSag>& getConditionSag() { return m_sags; }
    LoadingCondition getLoadingCondition() { return m_loadingCondition; }

private:
    CableConditionType m_type = CableConditionType_Other;
    QString m_name;
    QString m_temperature; // oC
    QString m_windPressure; // kgf/m2
    QString m_ultimateLoading; // ultimateLoading +(%) / Sag -(m)
    LoadingCondition m_loadingCondition = LoadingCondition_None;

    // Cache
    QMap<QString, CableConditionSag> m_sags;
//    QMap<QString, CableConditionResult> m_sagResults;

    friend class ProfileGraph;
    friend class CableSwing;
    friend class SagTension;
    friend class SagTensionSubstation;
    friend class tst_SagTension;
    friend class InsulationCoordination;
};

class Cable : public std::enable_shared_from_this<Cable>
{
public:
    explicit Cable(TransmissionLineWeakPtr transmissionLine);

    void load(QSettings *settings);
    void save(QSettings *settings, bool saveTemporary);

    bool validate(QString& error);

    int getId();
    QString getName();
    CableTypeWeakPtr getCableType() { return m_cableType; }
    InputOption getInputOption() { return m_inputOption; }
    double getSag(CableConditionType type, double span);
    double getCalculatedSag(CableConditionType type, const QString& span);
    QVector<CableSagPtr>& getSags() { return m_sags; }
    CableCondition *getGovernmentCondition(CableConditionType type);
    QVector<CableCondition*>& getGovernmentConditions() { return m_governingConditions; }
    CableCondition *getLoading(CableConditionType type);
    QVector<CableCondition*>& getLoadings() { return m_loadings; }
    QString getDescriptionText() { return m_description; }
    double getAverageSag() { return m_averageSag.toDouble(); }
    QString getAverageSagText() { return m_averageSag; }
    double getLongDurationSag() { return m_longDurationSag.toDouble(); }
    QString getLongDurationSagText() { return m_longDurationSag; }
    double getShortDurationSag() { return m_shortDurationSag.toDouble(); }
    QString getShortDurationSagText() { return m_shortDurationSag; }
    double getInitialTemperature() { return m_initialTemperature.toDouble(); }
    QString getInitialTemperatureText() { return m_initialTemperature; }
    double getFinalTemperature() { return m_finalTemperature.toDouble(); }
    QString getFinalTemperatureText() { return m_finalTemperature; }
    double getTemperatureInterval() { return m_temperatureInterval.toDouble(); }
    QString getTemperatureIntervalText() { return m_temperatureInterval; }
    double getSurfaceFactor() { return m_surfaceFactor.toDouble(); }
    QString getSurfaceFactorText() { return m_surfaceFactor; }
    double getEmissivityCoefficient() { return m_emissivityCoefficient.toDouble(); }
    QString getEmissivityCoefficientText() { return m_emissivityCoefficient; }
    double getSolarAbsorptivityCoefficient() { return m_solarAbsorptivityCoefficient.toDouble(); }
    QString getSolarAbsorptivityCoefficientText() { return m_solarAbsorptivityCoefficient; }

    void setCableType(const CableTypePtr& cableType) { m_cableType = cableType; }
    void setInputOption(const InputOption inputOption) { m_inputOption = inputOption; }
    void setDescriptionText(const QString& description) { m_description = description; }
    void setAverageSagText(const QString& averageSag) { m_averageSag = averageSag; }
    void setLongDurationSagText(const QString& longDurationSag) { m_longDurationSag = longDurationSag; }
    void setShortDurationSagText(const QString& shortDurationSag) { m_shortDurationSag = shortDurationSag; }
    void setInitialTemperature(const QString& temperature) { m_initialTemperature = temperature; }
    void setFinalTemperature(const QString& temperature) { m_finalTemperature = temperature; }
    void setTemperatureInterval(const QString& temperature) { m_temperatureInterval = temperature; }
    void setSurfaceFactorText(const QString& surfaceFactor) { m_surfaceFactor = surfaceFactor; }
    void setEmissivityCoefficientText(const QString& emissivityCoefficient) { m_emissivityCoefficient = emissivityCoefficient; }
    void setSolarAbsorptivityCoefficientText(const QString& solarAbsorptivityCoefficient) { m_solarAbsorptivityCoefficient = solarAbsorptivityCoefficient; }

    void addSpan(const QString &span);
    bool removeSpan(int index);
    void removeSpans();
    QString getSpan(int index);
    QVector<QString>& getSpans() { return m_spans; }
    bool moveSpans(int fromIndex, int toIndex);
    TransmissionLineWeakPtr getTransmissionLine() { return m_transmissionLine; }
    QString getErrorLocation();

private:
    CableTypeWeakPtr m_cableType;
    InputOption m_inputOption = InputOption_InputSags;
    QVector<CableSagPtr> m_sags;
    QVector<CableCondition*> m_governingConditions;
    QVector<CableCondition*> m_loadings;
    QVector<QString> m_spans;

    QString m_description;
    QString m_averageSag; // meters
    QString m_longDurationSag; // meters
    QString m_shortDurationSag; // meters
    QString m_initialTemperature; // oC
    QString m_finalTemperature; // oC
    QString m_temperatureInterval; // oC
    QString m_surfaceFactor;
    QString m_emissivityCoefficient;
    QString m_solarAbsorptivityCoefficient;

    TransmissionLineWeakPtr m_transmissionLine;

    // Cache
    QMap<QString, bool> m_hasFinalWithCreep;

    friend class SagTension;
};

#endif // CABLE_H
