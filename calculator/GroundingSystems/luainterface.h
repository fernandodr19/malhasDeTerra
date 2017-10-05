#ifndef LUAINTERFACE_H
#define LUAINTERFACE_H

#include <lua5.1/lua.hpp>
#include <string>
#include <QSettings>
#include <vector3d.h>

#define LUA_OK 0

class LuaInterface
{
public:
    bool initialize();
    void terminate();

    std::string validateString(const std::string& code);
    std::string executeString(std::string code, int nArguments = 0, int nResults = 0);
    std::string executeFile(const std::string& filename);

    lua_State *getLuaState() { return m_luaState; }

private:
    static bool popBoolean(lua_State *L);
    static std::string popString(lua_State *L);
    static int popInt(lua_State *L);
    static double popDouble(lua_State *L);
    static Vector3Dd popVector3D(lua_State *L);
    static std::complex<double> popComplex(lua_State *L);

    static void pushString(lua_State *L, const std::string& v);
    static void pushDouble(lua_State *L, double v);
    static void pushComplex(lua_State *L, std::complex<double> v);

    static int qtTranslate(lua_State* L);
    static int qtUpper(lua_State* L);
    static int calculateSurfaceVoltage(lua_State* L);
    static int calculateTouchPotentialLimit(lua_State* L);
    static int calculateStepPotentialLimit(lua_State* L);
    static int parallelImpedance(lua_State* L);
    static int complexSqrt(lua_State* L);
    static int getImageSize(lua_State* L);
    static int kCM(lua_State *L);

    template<typename T>
    static T getField(lua_State* L, int32_t arg, const std::string& key)
    {
        lua_getfield(L, arg, key.c_str());
        return getNumber<T>(L, -1);
    }

    template<typename T>
    static T getNumber(lua_State* L, int32_t arg)
    {
        return static_cast<T>(lua_tonumber(L, arg));
    }

    static void setField(lua_State* L, const char* arg, double v);

    lua_State *m_luaState = nullptr;
};

extern LuaInterface g_lua;

#endif // LUAINTERFACE_H
