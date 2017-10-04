#ifndef CABLE_H
#define CABLE_H

#include <QString>
#include <QMap>
#include "const.h"
#include <memory>

class QSettings;
class Project;

class Cable : public std::enable_shared_from_this<Cable>
{
public:
    explicit Cable();

    void load(QSettings *settings);
    void save(QSettings *settings, bool saveTemporary);

    bool validate(Project *project, QString& error);

    uint getId() { return m_id; }
    QString getCode() { return m_code; }
    CableCategory getCategory() { return m_category; }
    QString getType() { return m_type; }
    QString getSize() { return m_size; }
    QString getStranding() { return m_stranding; }
    double getDiameter() { return m_diameter.toDouble(); }
    QString getDiameterText() { return m_diameter; }
    QString getNumberWiresExternalLayerText() { return m_numberWiresExternalLayer; }
    QString getWiresDiameterText() { return m_wiresDiameter; }
    QString getRelativeWirePermeabilityText() { return m_relativeWirePermeability; }
    double getInitialElasticModulus() { return m_initialElasticModulus.toDouble(); }
    QString getInitialElasticModulusText() { return m_initialElasticModulus; }
    double getFinalElasticModulus() { return m_finalElasticModulus.toDouble(); }
    QString getFinalElasticModulusText() { return m_finalElasticModulus; }
    double getLinearExpansionCoeff() { return m_linearExpansionCoeff.toDouble(); }
    QString getLinearExpansionCoeffText() { return m_linearExpansionCoeff; }
    QString getConductivityText() { return m_conductivity; }
    double getACResistance() { return m_acResistance.toDouble(); }
    QString getACResistanceText() { return m_acResistance; }
    double getACResistanceTemperature() { return m_acResistanceTemperature.toDouble(); }
    QString getACResistanceTemperatureText() { return m_acResistanceTemperature; }
    double getACResistanceFrequency() { return m_acResistanceFrequency.toDouble(); }
    QString getACResistanceFrequencyText() { return m_acResistanceFrequency; }
    QString getACResistanceCurrentText() { return m_acResistanceCurrent; }
    double getDCResistance() { return m_dcResistance.toDouble(); }
    QString getDCResistanceText() { return m_dcResistance; }
    QString getDCResistanceTemperatureText() { return m_dcResistanceTemperature; }
    double getInternalReactance1m();
    double getInternalReactance1ft() { return m_internalReactance1ft.toDouble(); }
    QString getInternalReactance1ftText() { return m_internalReactance1ft; }
    double getInternalReactance1ftFrequency() { return m_internalReactance1ftFrequency.toDouble(); }
    QString getInternalReactance1ftFrequencyText() { return m_internalReactance1ftFrequency; }
    QString getInternalReactance1ftCurrentText() { return m_internalReactance1ftCurrent; }
    double getCrossSection() { return m_crossSection.toDouble(); }
    QString getCrossSectionText() { return m_crossSection; }
    double getLinearWeight() { return m_linearWeight.toDouble(); }
    QString getLinearWeightText() { return m_linearWeight; }
    double getUnitPrice() { return m_unitPrice.toDouble(); }
    QString getUnitPriceText() { return m_unitPrice; }
    double getTensileStrength() { return m_tensileStrength.toDouble(); }
    QString getTensileStrengthText() { return m_tensileStrength; }
    double getCurrentCapacity() { return m_currentCapacity.toDouble(); }
    QString getCurrentCapacityText() { return m_currentCapacity; }
    CableSaleUnit getSaleUnit() { return m_saleUnit; }

    bool isConductor() { return m_category == CableCategory_Conductor; }
    bool isShieldWire() { return m_category == CableCategory_ShieldWire; }
    bool isSteel();

    void setId(uint id) { m_id = id; }
    void setCode(const QString& code) { m_code = code; }
    void setCategory(CableCategory category) { m_category = category; }
    void setType(const QString& type) { m_type = type; }
    void setSize(const QString& size) { m_size = size; }
    void setStranding(const QString& stranding) { m_stranding = stranding; }
    void setDiameterText(const QString& diameter) { m_diameter = diameter; }
    void setNumberWiresExternalLayerText(const QString& numberWiresExternalLayer) { m_numberWiresExternalLayer = numberWiresExternalLayer; }
    void setWiresDiameterText(const QString& wiresDiameter) { m_wiresDiameter = wiresDiameter; }
    void setRelativeWirePermeabilityText(const QString& relativeWirePermeability) { m_relativeWirePermeability = relativeWirePermeability; }
    void setInitialElasticModulusText(const QString& elasticModulus) { m_initialElasticModulus = elasticModulus; }
    void setFinalElasticModulusText(const QString& elasticModulus) { m_finalElasticModulus = elasticModulus; }
    void setLinearExpansionCoeff(const QString& linearExpansionCoeff) { m_linearExpansionCoeff = linearExpansionCoeff; }
    void setConductivityText(const QString& conductivity) { m_conductivity = conductivity; }
    void setACResistanceText(const QString& acResistance) { m_acResistance = acResistance; }
    void setACResistanceTemperatureText(const QString& acResistanceTemperature) { m_acResistanceTemperature = acResistanceTemperature; }
    void setACResistanceFrequencyText(const QString& acResistanceFrequency) { m_acResistanceFrequency = acResistanceFrequency; }
    void setACResistanceCurrentText(const QString& acResistanceCurrent) { m_acResistanceCurrent = acResistanceCurrent; }
    void setDCResistanceText(const QString& dcResistance) { m_dcResistance = dcResistance; }
    void setDCResistanceTemperatureText(const QString& dcResistanceTemperature) { m_dcResistanceTemperature = dcResistanceTemperature; }
    void setInternalReactance1ftText(const QString& internalReactance1ft) { m_internalReactance1ft = internalReactance1ft; }
    void setInternalReactance1ftFrequencyText(const QString& internalReactance1ftFrequency) { m_internalReactance1ftFrequency = internalReactance1ftFrequency; }
    void setInternalReactance1ftCurrentText(const QString& internalReactance1ftCurrent) { m_internalReactance1ftCurrent = internalReactance1ftCurrent; }
    void setCrossSectionText(const QString& crossSection) { m_crossSection = crossSection; }
    void setLinearWeightText(const QString& linearWeight) { m_linearWeight = linearWeight; }
    void setUnitPriceText(const QString& unitPrice) { m_unitPrice = unitPrice; }
    void setTensileStrengthText(const QString& tensileStrength) { m_tensileStrength = tensileStrength; }
    void setCurrentCapacity(const QString& currentCapacity) { m_currentCapacity = currentCapacity; }
    void setSaleUnit(const CableSaleUnit saleUnit) { m_saleUnit = saleUnit; }

    QString getTranslatedType();

    static uint uidGen;

private:
    uint m_id = 0;
    QString m_code;
    CableCategory m_category;
    QString m_type;
    QString m_size;
    QString m_stranding;
    QString m_diameter;
    QString m_numberWiresExternalLayer;
    QString m_wiresDiameter;
    QString m_relativeWirePermeability;
    QString m_initialElasticModulus;
    QString m_finalElasticModulus;
    QString m_linearExpansionCoeff;
    QString m_conductivity;
    QString m_acResistance;
    QString m_acResistanceTemperature;
    QString m_acResistanceFrequency;
    QString m_acResistanceCurrent;
    QString m_dcResistance;
    QString m_dcResistanceTemperature;
    QString m_internalReactance1ft;
    QString m_internalReactance1ftFrequency;
    QString m_internalReactance1ftCurrent;
    QString m_crossSection;
    QString m_linearWeight;
    QString m_unitPrice;
    QString m_tensileStrength;
    QString m_currentCapacity;
    CableSaleUnit m_saleUnit;

};

#endif // CABLE_H
