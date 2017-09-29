#ifndef CONST_H
#define CONST_H

#define EXIT_NORMAL 0
#define EXIT_RESTART 1

#include <memory>

// Do not change the order.
// Mark as deprecated if not used anymore.

enum CableCategory {
    CableCategory_Conductor,
    CableCategory_ShieldWire,
};

enum TerrainCategory {
    TerrainCategory_A = 0,
    TerrainCategory_B,
    TerrainCategory_C,
    TerrainCategory_D,
};

enum InputOption {
    InputOption_InputSags = 0,
    InputOption_CalculateSags,
};

enum IntegrationTimeUnit {
    IntegrationTimeUnit_Seconds = 0,
    IntegrationTimeUnit_Minutes,
};

enum PhaseType {
    PhaseType_A = 0,
    PhaseType_B,
    PhaseType_C,
    PhaseType_Last,
};

enum SagType {
    AverageSag = 0,
    ShortDurationSag,
    LongDurationSag,
};

enum Weather {
    FairWeatherL50 = 0,
    FoulWeatherL50,
    FoulWeatherL5,
    FoulWeatherL1,
};

enum Detector {
    Detector_QuasiPeak = 0,
    Detector_Peak,
    Detector_Rms,
    Detector_Average
};

enum MaterialType {
    MaterialType_Insulating,
    MaterialType_Metallic,
};

enum ProjectType {
    Design = 0,
    Executive
};

enum LoadingCondition {
    LoadingCondition_None = 0,
    LoadingCondition_Initial,
    LoadingCondition_Final,
    LoadingCondition_Last
};

enum CableConditionType { // Cant change order. Mark as deprecated if needed
    CableConditionType_Other = 0,
    CableConditionType_MinimumTemperature,
    CableConditionType_MaximumWind,
    CableConditionType_ExtremeWind,
    CableConditionType_EDS,
    CableConditionType_AtmosphericDischargesSwing,
    CableConditionType_SwitchingSurgeSwing,
    CableConditionType_PowerFrequencySwing,
    CableConditionType_RightOfWaySwing,
    CableConditionType_AverageTemperature,
    CableConditionType_LongDurationTemperature,
    CableConditionType_ShortDurationTemperature,
    CableConditionType_StringingTemperature,
    CableConditionType_Creep,
    CableConditionType_Offset,
    CableConditionType_Last,
};

enum BroadbandMethod {
    BroadbandMethod_EPRI1975 = 0,
    BroadbandMethod_EPRI2005 = 1,
    BroadbandMethod_BPA = 2,
};

enum Direction {
    Direction_Undefined = -1,
    Direction_Left,
    Direction_Right,
};

enum CableTypeSaleUnit
{
    CableTypeSaleUnit_Kilometer = 0,
    CableTypeSaleUnit_Tonne,
};

enum TowerTypeType
{
    TowerTypeType_CrossRope = 0, //guyed
    TowerTypeType_MonomastroDelta, //guyed
    TowerTypeType_MonomastroTriangular, //guyed
    TowerTypeType_GuyedV, //guyed
    TowerTypeType_SelfSupportingHorizontal,
    TowerTypeType_SelfSupportingDelta,
    TowerTypeType_SelfSupportingTriangular,
    TowerTypeType_Other,
};

enum TowerTypeTypeByInsulator {
    TowerTypeTypeByInsulator_Suspension = 0,
    TowerTypeTypeByInsulator_Tension,
//    TowerTypeCategory_Transposition,
//    TowerTypeCategory_Terminal,
};

enum TowerTypeTypeBySupport {
    TowerTypeTypeBySupport_Guyed = 0,
    TowerTypeTypeBySupport_SelfSupporting,
};

class CableType;
typedef std::shared_ptr<CableType> CableTypePtr;
typedef std::weak_ptr<CableType> CableTypeWeakPtr;
class InsulatorType;
typedef std::shared_ptr<InsulatorType> InsulatorTypePtr;
typedef std::weak_ptr<InsulatorType> InsulatorTypeWeakPtr;
class Document;
typedef std::shared_ptr<Document> DocumentPtr;
typedef std::weak_ptr<Document> DocumentWeakPtr;
class GroundingSystem;
typedef std::shared_ptr<GroundingSystem> GroundingSystemPtr;
typedef std::weak_ptr<GroundingSystem> GroundingSystemWeakPtr;
class Pipeline;
typedef std::shared_ptr<Pipeline> PipelinePtr;
typedef std::weak_ptr<Pipeline> PipelineWeakPtr;
class Substation;
typedef std::shared_ptr<Substation> SubstationPtr;
typedef std::weak_ptr<Substation> SubstationWeakPtr;
class TransmissionLine;
typedef std::shared_ptr<TransmissionLine> TransmissionLinePtr;
typedef std::weak_ptr<TransmissionLine> TransmissionLineWeakPtr;
class Cable;
typedef std::shared_ptr<Cable> CablePtr;
typedef std::weak_ptr<Cable> CableWeakPtr;
class CableSag;
typedef std::shared_ptr<CableSag> CableSagPtr;
typedef std::weak_ptr<CableSag> CableSagWeakPtr;
class TowerType;
typedef std::shared_ptr<TowerType> TowerTypePtr;
typedef std::weak_ptr<TowerType> TowerTypeWeakPtr;
class InsulatorStringType;
typedef std::shared_ptr<InsulatorStringType> InsulatorStringTypePtr;
typedef std::weak_ptr<InsulatorStringType> InsulatorStringTypeWeakPtr;
class InsulatorStringTypePart;
typedef std::shared_ptr<InsulatorStringTypePart> InsulatorStringTypePartPtr;
typedef std::weak_ptr<InsulatorStringTypePart> InsulatorStringTypePartWeakPtr;
class TransmissionLineParalelism;
typedef std::shared_ptr<TransmissionLineParalelism> TransmissionLineParalelismPtr;
typedef std::weak_ptr<TransmissionLineParalelism> TransmissionLineParalelismWeakPtr;

#endif // CONST_H

