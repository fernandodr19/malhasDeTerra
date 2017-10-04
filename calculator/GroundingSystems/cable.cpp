#include "cable.h"
//#include <project.h>
#include <tmath.h>
#include <QDebug>
#include <QSettings>

uint Cable::uidGen = 1;

Cable::Cable()
{
    m_code = QObject::tr("New cable");
    m_category = CableCategory_Conductor;
}

void Cable::load(QSettings *settings)
{
    m_id = settings->value("id", 0).toUInt();
    m_code = settings->value("code").toString();
    m_category = (CableCategory)settings->value("category").toInt();
    m_type = settings->value("type").toString();
    m_size = settings->value("size").toString();
    m_stranding = settings->value("stranding").toString();
    m_diameter = settings->value("diameter").toString();
    m_numberWiresExternalLayer = settings->value("numberWiresExternalLayer").toString();
    m_wiresDiameter = settings->value("wiresDiameter").toString();
    m_relativeWirePermeability = settings->value("relativeWirePermeability").toString();
    m_initialElasticModulus = settings->value("initialElasticModulus").toString();
    m_finalElasticModulus = settings->value("finalElasticModulus").toString();
    m_linearExpansionCoeff = settings->value("linearExpansionCoeff").toString();
    m_conductivity = settings->value("conductivity").toString();
    m_acResistance = settings->value("acResistance").toString();
    m_acResistanceTemperature = settings->value("acResistanceTemperature").toString();
    m_acResistanceFrequency = settings->value("acResistanceFrequency").toString();
    m_acResistanceCurrent = settings->value("acResistanceCurrent").toString();
    m_dcResistance = settings->value("dcResistance").toString();
    m_dcResistanceTemperature = settings->value("dcResistanceTemperature").toString();
    m_internalReactance1ft = settings->value("internalReactance1ft").toString();
    m_internalReactance1ftFrequency = settings->value("internalReactance1ftFrequency").toString();
    m_internalReactance1ftCurrent = settings->value("internalReactance1ftCurrent").toString();
    m_crossSection = settings->value("crossSection").toString();
    m_linearWeight = settings->value("linearWeight").toString();
    m_unitPrice = settings->value("unitPrice").toString();
    m_tensileStrength = settings->value("tensileStrength").toString();
    m_currentCapacity = settings->value("currentCapacity").toString();
    m_saleUnit = (CableSaleUnit)settings->value("saleUnit", CableSaleUnit_Kilometer).toInt();
}

void Cable::save(QSettings *settings, bool saveTemporary)
{
    settings->setValue("id", m_id);
    settings->setValue("code", m_code);
    settings->setValue("category", (int)m_category);
    settings->setValue("type", m_type);
    settings->setValue("size", m_size);
    settings->setValue("stranding", m_stranding);
    settings->setValue("diameter", m_diameter);
    settings->setValue("numberWiresExternalLayer", m_numberWiresExternalLayer);
    settings->setValue("wiresDiameter", m_wiresDiameter);
    settings->setValue("relativeWirePermeability", m_relativeWirePermeability);
    settings->setValue("initialElasticModulus", m_initialElasticModulus);
    settings->setValue("finalElasticModulus", m_finalElasticModulus);
    settings->setValue("linearExpansionCoeff", m_linearExpansionCoeff);
    settings->setValue("conductivity", m_conductivity);
    settings->setValue("acResistance", m_acResistance);
    settings->setValue("acResistanceTemperature", m_acResistanceTemperature);
    settings->setValue("acResistanceFrequency", m_acResistanceFrequency);
    settings->setValue("acResistanceCurrent", m_acResistanceCurrent);
    settings->setValue("dcResistance", m_dcResistance);
    settings->setValue("dcResistanceTemperature", m_dcResistanceTemperature);
    settings->setValue("internalReactance1ft", m_internalReactance1ft);
    settings->setValue("internalReactance1ftFrequency", m_internalReactance1ftFrequency);
    settings->setValue("internalReactance1ftCurrent", m_internalReactance1ftCurrent);
    settings->setValue("crossSection", m_crossSection);
    settings->setValue("linearWeight", m_linearWeight);
    settings->setValue("unitPrice", m_unitPrice);
    settings->setValue("tensileStrength", m_tensileStrength);
    settings->setValue("currentCapacity", m_currentCapacity);
    settings->setValue("saleUnit", m_saleUnit);

    if(saveTemporary) {
        settings->setValue("isSteel", isSteel());
        settings->setValue("translatedType", getTranslatedType());
    }
}

bool Cable::validate(Project *project, QString& error)
{
//    bool ok;

//    double diameter = m_diameter.toDouble(&ok);
//    if(!ok || diameter <= 0) {
//        error += QObject::tr("Diameter of cable %1 is invalid.").arg(m_code) + "\n";
//        return false;
//    }

//    if(project->hasModule(Application::Module_CableACResistance)) {
//        double acResistance = m_acResistance.toDouble(&ok);
//        if(!ok || acResistance <= 0) {
//            error += QObject::tr("AC resistance of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }

//        double acResistanceTemperature = m_acResistanceTemperature.toDouble(&ok);
//        if(!ok || acResistanceTemperature <= 0) {
//            error += QObject::tr("AC resistance temperature of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }

//        double acResistanceFrequency = m_acResistanceFrequency.toDouble(&ok);
//        if(!ok || acResistanceFrequency <= 0) {
//            error += QObject::tr("AC resistance frequency of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }

//        double acResistanceCurrent = m_acResistanceCurrent.toDouble(&ok);
//        if((isSteel() && (m_acResistanceCurrent.isEmpty() || !ok || acResistanceCurrent <= 0)) || (!isSteel() && !m_acResistanceCurrent.isEmpty())) {
//            error += QObject::tr("AC resistance current of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }
//    }

//    if(project->hasModule(Application::Module_CableInternalReactance)) {
//        double internalReactance1ft = m_internalReactance1ft.toDouble(&ok);
//        if(!ok || internalReactance1ft <= 0) {
//            error += QObject::tr("Internal reactance of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }

//        double internalReactance1ftFrequency = m_internalReactance1ftFrequency.toDouble(&ok);
//        if(!ok || internalReactance1ftFrequency <= 0) {
//            error += QObject::tr("Internal reactance frequency of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }

//        double internalReactance1ftCurrent = m_internalReactance1ftCurrent.toDouble(&ok);
//        if((isSteel() && (m_internalReactance1ftCurrent.isEmpty() || !ok || internalReactance1ftCurrent <= 0)) || (!isSteel() && !m_internalReactance1ftCurrent.isEmpty())) {
//            error += QObject::tr("Internal reactance current of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }
//    }

//    if(project->hasModule(Application::Module_CableCrossSection)) {
//        double crossSection = m_crossSection.toDouble(&ok);
//       if(m_crossSection.isEmpty() || !ok || crossSection <= 0) {
//           error += QObject::tr("Cross section of cable %1 is invalid.").arg(m_code) + "\n";
//           return false;
//       }
//    }

//    if(project->hasModule(Application::Module_CableLinearWeight)) {
//        double linearWeight = m_linearWeight.toDouble(&ok);
//        if(m_linearWeight.isEmpty() || !ok || linearWeight <= 0) {
//            error += QObject::tr("Linear weight of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }
//    }

//    if(project->hasModule(Application::Module_CableTensileStrength)) {
//        double tensileStrength = m_tensileStrength.toDouble(&ok);
//        if(m_tensileStrength.isEmpty() || !ok || tensileStrength <= 0) {
//            error += QObject::tr("Tensile strength of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }
//    }

//    if(project->hasModule(Application::Module_CableInitialElasticModulus)) {
//        double initialElasticModulus = m_initialElasticModulus.toDouble(&ok);
//        if(m_initialElasticModulus.isEmpty() || !ok || initialElasticModulus <= 0) {
//            error += QObject::tr("Initial elastic modulus of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }
//    }

//    if(project->hasModule(Application::Module_CableLinearExpansionCoeff)) {
//        double linearExpansionCoeff = m_linearExpansionCoeff.toDouble(&ok);
//        if(m_linearExpansionCoeff.isEmpty() || !ok || linearExpansionCoeff <= 0) {
//            error += QObject::tr("Coefficient of linear expansion of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }
//    }

//    if(project->hasModule(Application::Module_CableCurrentCapacity) && isShieldWire()) {
//        double cableCapacity = m_currentCapacity.toDouble(&ok);
//        if(!ok || cableCapacity < 0) {
//            error += QObject::tr("Current capacity of cable %1 is invalid.").arg(m_code) + "\n";
//            return false;
//        }
//    }

    return true;
}

double Cable::getInternalReactance1m()
{
    double f = getInternalReactance1ftFrequency();
    return getInternalReactance1ft() - std::log(math::feet_to_meter) * math::u0 * f * 1000;
}

bool Cable::isSteel()
{
    if(m_type == "Galvanized steel HS")
        return true;
    else if(m_type == "Galvanized steel EHS")
        return true;
    else
        return false;
}

QString Cable::getTranslatedType()
{
    if(m_type == "AAC")
        return QObject::tr("AAC");
    else if(m_type == "AAAC")
        return QObject::tr("AAAC");
    else if(m_type == "AAAC 1120")
        return QObject::tr("AAAC 1120");
    else if(m_type == "AAAC 6201")
        return QObject::tr("AAAC 6201");
    else if(m_type == "ACAR")
        return QObject::tr("ACAR");
    else if(m_type == "ACSR")
        return QObject::tr("ACSR");
    else if(m_type == "Alumoweld")
        return QObject::tr("Alumoweld");
    else if(m_type == "Copperweld")
        return QObject::tr("Copperweld");
    else if(m_type == "Copper")
        return QObject::tr("Copper");
    else if(m_type == "Galvanized steel HS")
        return QObject::tr("Galvanized steel HS");
    else if(m_type == "Galvanized steel EHS")
        return QObject::tr("Galvanized steel EHS");
    else if(m_type == "OPGW")
        return QObject::tr("OPGW");
    else if(m_type == "Other")
        return QObject::tr("Other");
    else
        return m_type;
}
