#include "cable.h"
#include <cabletype.h>
#include <QObject>
//#include <core/insulatorstringtype.h>

CableCondition::CableCondition()
{

}

bool CableSag::validate(QString& error, int i)
{
    bool ok;
    double sag = m_sag.toDouble(&ok);
    if(!ok || sag < 0) {
        error += QObject::tr("Sag %1 is invalid.").arg(i+1) + "\n";
        return false;
    }

    double span = m_span.toDouble(&ok);
    if(!ok || span < 0) {
        error += QObject::tr("Span %1 is invalid.").arg(i+1) + "\n";
        return false;
    }

    return true;
}

void CableCondition::load(QSettings *settings)
{
//    m_type = (CableConditionType)settings->value("type", CableConditionType_Other).toInt();
//    m_name = settings->value("name").toString();
//    m_temperature = settings->value("temperature").toString();
//    m_windPressure = settings->value("windPressure").toString();
//    m_ultimateLoading = settings->value("ultimateLoading").toString();
//    m_loadingCondition = (LoadingCondition)settings->value("loadingCondition").toInt();
}

void CableCondition::save(QSettings *settings, bool saveTemporary)
{
//    settings->setValue("type", m_type);
//    settings->setValue("name", m_name);
//    settings->setValue("temperature", m_temperature);
//    settings->setValue("windPressure", m_windPressure);
//    settings->setValue("ultimateLoading", m_ultimateLoading);
//    settings->setValue("loadingCondition", m_loadingCondition);

//    if(saveTemporary) {
//        settings->beginGroup("Sags");
//        settings->setValue("_isKeyArray", true);
//        for(auto it = m_sags.begin(); it != m_sags.end(); ++it) {
//            CableConditionSag& c = it.value();
//            settings->beginGroup(it.key());
//            settings->setValue("initialHorizontalTension", c.initialHorizontalTension);
//            settings->setValue("initialTangentialTension", c.initialTangentialTension);
//            settings->setValue("initialSag", c.initialSag);
//            settings->setValue("finalHorizontalTension", c.finalHorizontalTension);
//            settings->setValue("finalTangentialTension", c.finalTangentialTension);
//            settings->setValue("finalSag", c.finalSag);
//            settings->endGroup();
//        }
//        settings->endGroup();

//        settings->beginGroup("SagResults");
//        settings->setValue("_isKeyArray", true);
//        for(auto it = m_sagResults.begin(); it != m_sagResults.end(); ++it) {
//            CableConditionResult& c = it.value();
//            settings->beginGroup(it.key());
//            settings->setValue("horizontalTension", c.horizontalTension);
//            settings->setValue("cableSag", c.cableSag);
//            settings->setValue("chainSag", c.chainSag);
//            settings->setValue("totalSag", c.totalSag);

//            settings->setValue("catenaryMaximumSagX", c.catenaryMaximumSagX);
//            settings->setValue("catenaryMaximumSagY", c.catenaryMaximumSagY);
//            settings->setValue("catenaryLowerPointX", c.catenaryLowerPointX);
//            settings->setValue("catenaryLowerPointY", c.catenaryLowerPointY);

//            settings->beginGroup("LeftChain");
//            c.cadL->save(settings, saveTemporary);
//            settings->endGroup();

//            settings->beginGroup("RightChain");
//            c.cadR->save(settings, saveTemporary);
//            settings->endGroup();

//            settings->endGroup();
//        }
//        settings->endGroup();
//    }
}

bool CableCondition::validate(Project *project, QString &error, bool governmentCondition, int i)
{
    bool ok;
    QString tableName;
    if(governmentCondition)
        tableName = QObject::tr("government condition");
    else
        tableName = QObject::tr("loadings");

    m_temperature.toDouble(&ok);
    if(!ok) {
        error += QObject::tr("Temperature of %1 %2 is invalid.").arg(tableName).arg(i+1) + "\n";
        return false;
    }

    if(true/*project->hasModule(Application::Module_CableWindPressure)*/) {
        double windPressure = m_windPressure.toDouble(&ok);
        if(!ok || windPressure < 0) {
            error += QObject::tr("Wind pressure of %1 %2 is invalid.").arg(tableName).arg(i+1) + "\n";
            return false;
        }
    }

    if(governmentCondition) {
        m_ultimateLoading.toDouble(&ok);
        if(!ok) {
            error += QObject::tr("Ultimate loading of government condition %1 is invalid.").arg(i+1) + "\n";
            return false;
        }
    }

    return true;
}

Cable::Cable(TransmissionLineWeakPtr transmissionLine)
{
    m_transmissionLine = transmissionLine;
}

void Cable::load(QSettings *settings)
{
//    uint cableTypeId = settings->value("cableId", 0).toUInt();
//    m_cableType = g_database->getCableById(cableTypeId);
//    m_inputOption = static_cast<InputOption>(settings->value("inputOption").toInt());

//    int nSags = settings->beginReadArray("Sags");
//    for(int i = 0; i < nSags; ++i) {
//        settings->setArrayIndex(i);
//        CableSagPtr cableSag = std::make_shared<CableSag>();
//        cableSag->setType((CableConditionType)settings->value("type").toInt());
//        cableSag->setSag(settings->value("sag").toString());
//        cableSag->setSpan(settings->value("span").toString());
//        m_sags.push_back(cableSag);
//    }
//    settings->endArray();

//    settings->beginGroup("GoverningConditions");
//    m_governingConditions.clear();
//    for(const QString& group : settings->childGroups()) {
//        settings->beginGroup(group);
//        CableCondition *cableCondition = new CableCondition();
//        cableCondition->load(settings);
//        m_governingConditions.push_back(cableCondition);
//        settings->endGroup();
//    }
//    settings->endGroup();

//    settings->beginGroup("Loadings");
//    m_loadings.clear();
//    for(const QString& group : settings->childGroups()) {
//        settings->beginGroup(group);
//        CableCondition *cableCondition = new CableCondition();
//        cableCondition->load(settings);
//        m_loadings.push_back(cableCondition);
//        settings->endGroup();
//    }
//    settings->endGroup();

//    removeSpans();
//    Settings::loadVector(settings, "Spans", m_spans);

//    m_description = settings->value("description").toString();
//    m_averageSag = settings->value("averageSag").toString();
//    m_longDurationSag = settings->value("longDurationSag").toString();
//    m_shortDurationSag = settings->value("shortDurationSag").toString();
//    m_initialTemperature = settings->value("initialTemperature").toString();
//    m_finalTemperature = settings->value("finalTemperature").toString();
//    m_temperatureInterval = settings->value("temperatureInterval").toString();
//    m_surfaceFactor = settings->value("surfaceFactor").toString();
//    m_emissivityCoefficient = settings->value("emissivityCoefficient").toString();
//    m_solarAbsorptivityCoefficient = settings->value("solarAbsorptivityCoefficient").toString();
}

void Cable::save(QSettings *settings, bool saveTemporary)
{
//    if(CableTypePtr cableType = m_cableType.lock())
//        settings->setValue("cableId", cableType->getId());
//    else
//        settings->setValue("cableId", 0);
//    settings->setValue("inputOption", m_inputOption);

//    settings->beginWriteArray("Sags");
//    settings->setValue("_isArray", true);
//    for(int i = 0; i < m_sags.size(); ++i) {
//        settings->setArrayIndex(i);
//        CableSagPtr cableSag = m_sags[i];
//        settings->setValue("type", cableSag->getType());
//        settings->setValue("sag", cableSag->getSag());
//        settings->setValue("span", cableSag->getSpan());
//    }
//    settings->endArray();

//    settings->beginGroup("GoverningConditions");
//    settings->setValue("_isArray", true);
//    int gcCount = m_governingConditions.size();
//    int gcDigits = (int)std::log10(gcCount) + 1;
//    for(int i = 0; i < gcCount; ++i) {
//        CableCondition *cableCondition = m_governingConditions[i];
//        settings->beginGroup(QString("GoverningCondition%1").arg(i, gcDigits, 10, QChar('0')));
//        cableCondition->save(settings, saveTemporary);
//        settings->endGroup();
//    }
//    settings->endGroup();

//    settings->beginGroup("Loadings");
//    settings->setValue("_isArray", true);
//    int lCount = m_loadings.size();
//    int lDigits = (int)std::log10(lCount) + 1;
//    for(int i = 0; i < lCount; ++i) {
//        CableCondition *cableCondition = m_loadings[i];
//        settings->beginGroup(QString("Loading%1").arg(i, lDigits, 10, QChar('0')));
//        cableCondition->save(settings, saveTemporary);
//        settings->endGroup();
//    }
//    settings->endGroup();

//    Settings::saveVector(settings, "Spans", m_spans);

//    settings->setValue("description", m_description);
//    settings->setValue("averageSag", m_averageSag);
//    settings->setValue("longDurationSag", m_longDurationSag);
//    settings->setValue("shortDurationSag", m_shortDurationSag);
//    settings->setValue("initialTemperature", m_initialTemperature);
//    settings->setValue("finalTemperature", m_finalTemperature);
//    settings->setValue("temperatureInterval", m_temperatureInterval);
//    settings->setValue("surfaceFactor", m_surfaceFactor);
//    settings->setValue("emissivityCoefficient", m_emissivityCoefficient);
//    settings->setValue("solarAbsorptivityCoefficient", m_solarAbsorptivityCoefficient);

//    if(saveTemporary)
//        Settings::saveMap(settings, "hasFinalWithCreep", m_hasFinalWithCreep);
}

bool Cable::validate(QString& error)
{
    CableTypePtr cableType = m_cableType.lock();
    if(!cableType) {
        error += QObject::tr("There is no cable selected.") + getErrorLocation() + "\n";
        return false;
    }

    Project *project = nullptr;
    if(TransmissionLinePtr tl = getTransmissionLine().lock()) {
//        project = tl->getProject();
    } else
        return false;

    if(!cableType->validate(project, error))
        return false;

    bool ok;

    if(true/*project->hasModule(Application::Module_CableInputOption)*/ && m_inputOption == InputOption_InputSags) {
        for(int i = 0; i < m_sags.size(); i++) {
            if(!m_sags[i]->validate(error, i)) {
                error += getErrorLocation();
                return false;
            }
        }
    }

    if(true/*project->hasModule(Application::Module_CableGoverningConditions)*/ && (!true/*project->hasModule(Application::Module_CableInputOption)*/ || m_inputOption == InputOption_CalculateSags)) {
        for(int i = 0; i < m_governingConditions.size(); i++) {
            if(!m_governingConditions[i]->validate(project, error, true, i)) {
                error += getErrorLocation();
                return false;
            }
        }
    }

    if(true/*project->hasModule(Application::Module_CableLoadings)*/ && (!true/*project->hasModule(Application::Module_CableInputOption)*/ || m_inputOption == InputOption_CalculateSags)) {
        for(int i = 0; i < m_loadings.size(); i++) {
            if(!m_loadings[i]->validate(project, error, false, i)) {
                error += getErrorLocation();
                return false;
            }
        }
    }

    if(true/*project->hasModule(Application::Module_CableSpans)*/) {
        for(int i = 0; i < m_spans.count(); i++) {
            double spanNumber = m_spans.at(i).toDouble(&ok);
            if(!ok || spanNumber <= 0) {
                error = QObject::tr("Span %1 is invalid.").arg(i+1) + getErrorLocation() + "\n";
                return false;
            }
        }
    }

//    if(project->hasModule(Application::Module_CableAverageSag)) {
//        double averageSag = getAverageSagText().toDouble(&ok);
//        if(!ok || averageSag < 0) {
//            error += QObject::tr("Average sag is invalid.") + getErrorLocation() + "\n";
//            return false;
//        }
//    }

//    if(cableType->isConductor() && project->hasModule(Application::Module_CableShortDurationSag)) {
//        double shortSag = getShortDurationSagText().toDouble(&ok);
//        if(!ok || shortSag < 0) {
//            error += QObject::tr("Short duration sag is invalid.") + getErrorLocation() + "\n";
//            return false;
//        }
//    }

//    if(cableType->isConductor() && project->hasModule(Application::Module_CableLongDurationSag)) {
//        double longSag = getLongDurationSagText().toDouble(&ok);
//        if(!ok || longSag < 0) {
//            error += QObject::tr("Long duration sag is invalid.") + getErrorLocation() + "\n";
//            return false;
//        }
//    }

//    if(cableType->isConductor() && project->hasModule(Application::Module_CableSurfaceFactor)) {
//        double surfaceFactor = m_surfaceFactor.toDouble(&ok);
//        if(!ok || surfaceFactor <= 0) {
//            error += QObject::tr("Surface factor is invalid.") + getErrorLocation() + "\n";
//            return false;
//        }
//    }

//    if(cableType->isConductor() && project->hasModule(Application::Module_CableTemperatures)) {
//        if(!(m_initialTemperature.isEmpty() && m_finalTemperature.isEmpty() && m_temperatureInterval.isEmpty())) {
//            if(m_initialTemperature.isEmpty() || m_finalTemperature.isEmpty() || m_temperatureInterval.isEmpty()) {
//                error += QObject::tr("You can either fill all fields of temperature or none of them.") + getErrorLocation() + "\n";
//                return false;
//            }
//            double initialTemperature = m_initialTemperature.toDouble(&ok);
//            if(!ok) {
//                error += QObject::tr("Initial temperature is invalid.") + getErrorLocation() + "\n";
//                return false;
//            }

//            double finalTemperature = m_finalTemperature.toDouble(&ok);
//            if(!ok) {
//                error += QObject::tr("Final temperature is invalid.") + getErrorLocation() + "\n";
//                return false;
//            }

//            if(initialTemperature > finalTemperature) {
//                error += QObject::tr("Initial temperature must be smaller than final temperature.") + getErrorLocation() + "\n";
//                return false;
//            }

//            double intervalTemperature = m_temperatureInterval.toDouble(&ok);
//            if(!ok || intervalTemperature <= 0) {
//                error += QObject::tr("Interval temperature must be greater than 0.") + getErrorLocation() + "\n";
//                return false;
//            }

//            double nIntervals = (finalTemperature - initialTemperature) / intervalTemperature;
//            if(nIntervals >= 1000) {
//                error += QObject::tr("Too many temperatures, try increasing the interval or decreasing the difference between the initial and final temperatures.") + getErrorLocation() + "\n";
//                return false;
//            }
//        }
//    }

//    if(cableType->isConductor() && project->hasModule(Application::Module_CableEmissivityCoefficient)) {
//        double emissivityCorfficient = m_emissivityCoefficient.toDouble(&ok);
//        if(!ok || emissivityCorfficient < 0) {
//            error += QObject::tr("Emissivity coefficient is invalid.") + getErrorLocation() + "\n";
//            return false;
//        }
//    }

//    if(cableType->isConductor() && project->hasModule(Application::Module_CableSolarAbsoptivityCoefficient)) {
//        double solarAbsoptivityCoefficient = m_solarAbsorptivityCoefficient.toDouble(&ok);
//        if(!ok || solarAbsoptivityCoefficient < 0) {
//            error += QObject::tr("Conductor solar absoptivity coefficient is invalid.") + getErrorLocation() + "\n";
//            return false;
//        }
//    }

    return true;
}

int Cable::getId()
{
//    if(TransmissionLinePtr tl = m_transmissionLine.lock())
//        return tl->getCableIndex(shared_from_this());
    return -1;
}

QString Cable::getName()
{
    if(CableTypePtr cableType = m_cableType.lock()) {
        if(m_description.isEmpty())
            return cableType->getCode();
        else
            return cableType->getCode() + " (" + m_description + ")";
    }
    return "";
}

double Cable::getSag(CableConditionType type, double span)
{
    for(CableSagPtr sag : m_sags) {
        if(sag->getType() == type && span == sag->getSpan().toDouble())
            return sag->getSag().toDouble();
    }
    return -1;
}

double Cable::getCalculatedSag(CableConditionType type, const QString& span)
{
    for(CableCondition *cc : m_governingConditions) {
        if(cc->getType() == type)
            return cc->getConditionSag()[span].finalSag;
    }
    for(CableCondition *cc : m_loadings) {
        if(cc->getType() == type)
            return cc->getConditionSag()[span].finalSag;
    }
    return -1;
}

CableCondition *Cable::getGovernmentCondition(CableConditionType type)
{
    for(CableCondition *cc : m_governingConditions) {
        if(cc->getType() == type)
            return cc;
    }
    return nullptr;
}

CableCondition *Cable::getLoading(CableConditionType type)
{
    for(CableCondition *cc : m_loadings) {
        if(cc->getType() == type)
            return cc;
    }
    return nullptr;
}

void Cable::addSpan(const QString &span)
{
    m_spans.push_back(span);
}

bool Cable::removeSpan(int index)
{
    if(index < 0 || index >= m_spans.size())
        return false;

    auto it = m_spans.begin() + index;
    m_spans.erase(it);
    return true;
}

void Cable::removeSpans()
{
    m_spans.clear();
}

QString Cable::getSpan(int index)
{
    if(index < 0 || index >= m_spans.size())
        return nullptr;
    return m_spans[index];
}

bool Cable::moveSpans(int fromIndex, int toIndex)
{
    if(fromIndex < 0 || fromIndex >= m_spans.size() || toIndex < 0 || toIndex >= m_spans.size())
        return false;

    QString span = m_spans[fromIndex];
    m_spans.removeAt(fromIndex);
    m_spans.insert(toIndex, span);
    return true;
}

QString Cable::getErrorLocation()
{
    QString location = QObject::tr("\n\nAt:\n");

    if(CableTypePtr cable = getCableType().lock())
        location += QObject::tr("Cable") + ": " + cable->getCode();
    return location;
}
