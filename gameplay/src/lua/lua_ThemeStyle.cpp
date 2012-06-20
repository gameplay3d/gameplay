#include "Base.h"
#include "ScriptController.h"
#include "ThemeStyle.h"
#include "lua_ThemeStyle.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_ThemeStyle()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg* lua_members = NULL;
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("Theme");

    sc->registerClass("ThemeStyle", lua_members, NULL, NULL, lua_statics, scopePath);
}

static Theme::Style* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "ThemeStyle");
    luaL_argcheck(state, userdata != NULL, 1, "'ThemeStyle' expected.");
    return (Theme::Style*)((ScriptController::LuaObject*)userdata)->instance;
}

}
