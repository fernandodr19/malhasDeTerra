#include "luainterface.h"
#include <QtCore>
#include <QDebug>
#include <project.h>
#include "groundingsystem.h"
#include <QImage>
#include <QStringList>

LuaInterface g_lua;

bool LuaInterface::initialize()
{
    m_luaState = luaL_newstate();
    if(!m_luaState)
        return false;

    luaL_openlibs(m_luaState);

    lua_register(m_luaState, "qtTranslate", LuaInterface::qtTranslate);
    lua_register(m_luaState, "qtUpper", LuaInterface::qtUpper);
    lua_register(m_luaState, "calculateSurfaceVoltage", LuaInterface::calculateSurfaceVoltage);
    lua_register(m_luaState, "calculateTouchPotentialLimit", LuaInterface::calculateTouchPotentialLimit);
    lua_register(m_luaState, "calculateStepPotentialLimit", LuaInterface::calculateStepPotentialLimit);
    lua_register(m_luaState, "parallelImpedance", LuaInterface::parallelImpedance);
    lua_register(m_luaState, "complexSqrt", LuaInterface::complexSqrt);
    lua_register(m_luaState, "getImageSize", LuaInterface::getImageSize);
    lua_register(m_luaState, "kCM", LuaInterface::kCM);

    return true;
}

void LuaInterface::terminate()
{
    if(m_luaState)
        lua_close(m_luaState);
}

std::string LuaInterface::validateString(const std::string& code)
{
    if(luaL_loadstring(m_luaState, code.c_str()) != LUA_OK)
        return popString(m_luaState);

    lua_pop(m_luaState, 1);
    return std::string();
}

std::string LuaInterface::executeString(std::string code, int nArguments, int nResults)
{
    if(luaL_loadstring(m_luaState, code.c_str()) != LUA_OK)
        return popString(m_luaState);

    if(lua_pcall(m_luaState, nArguments, nResults, 0) != LUA_OK)
        return popString(m_luaState);

    if(nResults == 1)
        return popString(m_luaState);

    return std::string();
}

std::string LuaInterface::executeFile(const std::string& filename)
{
    //loads file as a chunk at stack top
    if(luaL_loadfile(m_luaState, filename.c_str()) != LUA_OK)
        return popString(m_luaState);

    //check that it is loaded as a function
    if(!lua_isfunction(m_luaState, -1))
        return "Script was not loaded as a function.";

    //execute it
    if(lua_pcall(m_luaState, 0, 0, 0) != LUA_OK)
        return popString(m_luaState);

    return std::string();
}

void LuaInterface::setField(lua_State* L, const char* arg, double v)
{
    lua_pushnumber(L, v);
    lua_setfield(L, -2, arg);
}

bool LuaInterface::popBoolean(lua_State *L)
{
    lua_pop(L, 1);
    if(lua_isboolean(L, 0))
        return lua_toboolean(L, 0);
    return false;
}

std::string LuaInterface::popString(lua_State *L)
{
    lua_pop(L, 1);
    const char* str = "";
    if(lua_isstring(L, 0)) {
        str = lua_tostring(L, 0);
    }
    return str;
}

int LuaInterface::popInt(lua_State *L)
{
    lua_pop(L, 1);
    int number = 0;
    if(lua_isnumber(L, 0))
        number = getNumber<int>(L, 0);

    return number;
}

double LuaInterface::popDouble(lua_State *L)
{
    lua_pop(L, 1);
    double number = 0;
    if(lua_isnumber(L, 0))
        number = getNumber<double>(L, 0);

    return number;
}

Vector3Dd LuaInterface::popVector3D(lua_State *L)
{
    int stack = lua_gettop(L);

    Vector3Dd vec;
    vec.setX(getField<double>(L, stack, "x"));
    vec.setY(getField<double>(L, stack, "y"));
    vec.setZ(getField<double>(L, stack, "z"));

    lua_pop(L, 4);
    return vec;
}

std::complex<double> LuaInterface::popComplex(lua_State *L)
{
    int stack = lua_gettop(L);

    std::complex<double> comp;
    comp.real(getField<double>(L, stack, "real"));
    comp.imag(getField<double>(L, stack, "imag"));

    lua_pop(L, 3);
    return comp;
}

void LuaInterface::pushString(lua_State *L, const std::string& v)
{
    lua_pushlstring(L, v.c_str(), v.length());
}

void LuaInterface::pushDouble(lua_State *L, double v)
{
    lua_pushnumber(L, v);
}

void LuaInterface::pushComplex(lua_State *L, std::complex<double> v)
{
    lua_createtable(L, 0, 4);

    setField(L, "real", v.real());
    setField(L, "imag", v.imag());
    setField(L, "abs", std::abs(v));
    setField(L, "arg", std::arg(v));
}

int LuaInterface::qtTranslate(lua_State* L)
{
    //qtTranslate(context, key)
    std::string key = popString(L);
    std::string context = popString(L);
    QString translated = QCoreApplication::translate(context.c_str(), key.c_str());
    pushString(L, translated.toStdString());
    return 1;
}

int LuaInterface::qtUpper(lua_State* L)
{
    //qtUpper(str)
    std::string str = popString(L);
    QString upper = QString(str.c_str()).toUpper();
    pushString(L, upper.toStdString());
    return 1;
}

int LuaInterface::calculateSurfaceVoltage(lua_State* L)
{
    //calculateSurfaceVoltage(index, vector)
    Vector3Dd vector3d = popVector3D(L);
    int index = popInt(L);

    GroundingSystemPtr groundingSystem = g_database->getGroundingSystem(index);
    if(groundingSystem) {
        double surfaceVoltage = groundingSystem->getSurfaceVoltage(vector3d);
        pushDouble(L, surfaceVoltage);
        return 1;
    }

    lua_pushnil(L);
    return 1;
}

int LuaInterface::calculateTouchPotentialLimit(lua_State* L)
{
    //calculateTouchPotentialLimit(index, faultTime, weight)
    int weight = popInt(L);
    double faultTime = popDouble(L);
    int index = popInt(L);

    GroundingSystemPtr groundingSystem = g_database->getGroundingSystem(index);
    if(groundingSystem) {
        double touchPotentialLimit = groundingSystem->calculateTouchPotentialLimit2013(faultTime, weight);
        pushDouble(L, touchPotentialLimit);
        return 1;
    }

    lua_pushnil(L);
    return 1;
}

int LuaInterface::calculateStepPotentialLimit(lua_State* L)
{
    //calculateStepPotentialLimit(index, faultTime, weight)
    int weight = popInt(L);
    double faultTime = popDouble(L);
    int index = popInt(L);

    GroundingSystemPtr groundingSystem = g_database->getGroundingSystem(index);
    if(groundingSystem) {
        double stepPotentialLimit = groundingSystem->calculateStepPotentialLimit2013(faultTime, weight);
        pushDouble(L, stepPotentialLimit);
        return 1;
    }

    lua_pushnil(L);
    return 1;
}

int LuaInterface::parallelImpedance(lua_State* L)
{
    //parallelImpedance(impedance, impedance2)
    std::complex<double> impedance2 = popComplex(L);
    std::complex<double> impedance = popComplex(L);
    std::complex<double> total = math::parallelImpedance(impedance, impedance2);

    pushComplex(L, total);
    return 1;
}

int LuaInterface::complexSqrt(lua_State* L)
{
    //complexSqrt(comp1)
    std::complex<double> comp1 = popComplex(L);
    std::complex<double> result = std::sqrt(comp1);

    pushComplex(L, result);
    return 1;
}

int LuaInterface::getImageSize(lua_State *L)
{
    QString url(popString(L).c_str());
    QString imgName(popString(L).c_str());

    QStringList urls;
    urls.push_back(url);
    urls.push_back("assets/reports/assets/");

    QImage img;

    for(QString url : urls) {
        img.load(url + imgName);

        if(!img.isNull())
            break;
    }

    if(img.isNull())
        return 0;

    lua_createtable(L, 0, 2);
    setField(L, "width", img.width());
    setField(L, "height", img.height());
    return 1;
}

int LuaInterface::kCM(lua_State *L)
{
    // kCM(value)

    QString value = popString(L).c_str();

    QStringList split = value.split(" ");
    if(split.size() != 2) {
        qDebug() << "[kCM()]. Unidade desconhecida. Valor:" << value;
        lua_pushnumber(L, -1);
        return 1;
    }

    QString number = split[0];
    QString unit = split[1];

    double returnValue = 0;

    if(unit == "AWG") {
        QStringList split2 = number.split("/");
        int n = split2.size() == 2 ? 1-split[0].toDouble() : split[0].toDouble();
        returnValue = std::pow(5 * std::pow(92, (36-(n))/39.0), 2) / 1000.0;
    }
    else if(unit == "kCM" || unit == "MCM") {
        returnValue = number.toDouble();
    }
    else
        qCritical() << "[kCM()]. Unidade desconhecida. Valor:" << value;

    lua_pushnumber(L, returnValue);
    return 1;
}
