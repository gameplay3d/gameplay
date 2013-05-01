#include "Base.h"
#include "ScriptController.h"
#include "lua_Form.h"
#include "AbsoluteLayout.h"
#include "Animation.h"
#include "AnimationTarget.h"
#include "Base.h"
#include "Button.h"
#include "CheckBox.h"
#include "Container.h"
#include "Control.h"
#include "FlowLayout.h"
#include "Form.h"
#include "Game.h"
#include "ImageControl.h"
#include "Joystick.h"
#include "Label.h"
#include "Layout.h"
#include "Node.h"
#include "RadioButton.h"
#include "Ref.h"
#include "Scene.h"
#include "ScriptController.h"
#include "ScriptTarget.h"
#include "Slider.h"
#include "TextBox.h"
#include "Theme.h"
#include "VerticalLayout.h"
#include "lua_ContainerScroll.h"
#include "lua_ControlAlignment.h"
#include "lua_ControlListenerEventType.h"
#include "lua_ControlState.h"
#include "lua_CurveInterpolationType.h"
#include "lua_FontJustify.h"
#include "lua_LayoutType.h"

namespace gameplay
{

void luaRegister_Form()
{
    const luaL_Reg lua_members[] = 
    {
        {"addControl", lua_Form_addControl},
        {"addListener", lua_Form_addListener},
        {"addRef", lua_Form_addRef},
        {"addScriptCallback", lua_Form_addScriptCallback},
        {"createAnimation", lua_Form_createAnimation},
        {"createAnimationFromBy", lua_Form_createAnimationFromBy},
        {"createAnimationFromTo", lua_Form_createAnimationFromTo},
        {"destroyAnimation", lua_Form_destroyAnimation},
        {"draw", lua_Form_draw},
        {"getAbsoluteBounds", lua_Form_getAbsoluteBounds},
        {"getAlignment", lua_Form_getAlignment},
        {"getAnimation", lua_Form_getAnimation},
        {"getAnimationPropertyComponentCount", lua_Form_getAnimationPropertyComponentCount},
        {"getAnimationPropertyValue", lua_Form_getAnimationPropertyValue},
        {"getAutoHeight", lua_Form_getAutoHeight},
        {"getAutoWidth", lua_Form_getAutoWidth},
        {"getBorder", lua_Form_getBorder},
        {"getBounds", lua_Form_getBounds},
        {"getClip", lua_Form_getClip},
        {"getClipBounds", lua_Form_getClipBounds},
        {"getConsumeInputEvents", lua_Form_getConsumeInputEvents},
        {"getControl", lua_Form_getControl},
        {"getCursorColor", lua_Form_getCursorColor},
        {"getCursorRegion", lua_Form_getCursorRegion},
        {"getCursorUVs", lua_Form_getCursorUVs},
        {"getFocusIndex", lua_Form_getFocusIndex},
        {"getFont", lua_Form_getFont},
        {"getFontSize", lua_Form_getFontSize},
        {"getHeight", lua_Form_getHeight},
        {"getId", lua_Form_getId},
        {"getImageColor", lua_Form_getImageColor},
        {"getImageRegion", lua_Form_getImageRegion},
        {"getImageUVs", lua_Form_getImageUVs},
        {"getLayout", lua_Form_getLayout},
        {"getMargin", lua_Form_getMargin},
        {"getOpacity", lua_Form_getOpacity},
        {"getPadding", lua_Form_getPadding},
        {"getRefCount", lua_Form_getRefCount},
        {"getScroll", lua_Form_getScroll},
        {"getScrollWheelRequiresFocus", lua_Form_getScrollWheelRequiresFocus},
        {"getScrollWheelSpeed", lua_Form_getScrollWheelSpeed},
        {"getScrollingFriction", lua_Form_getScrollingFriction},
        {"getSkinColor", lua_Form_getSkinColor},
        {"getSkinRegion", lua_Form_getSkinRegion},
        {"getState", lua_Form_getState},
        {"getStyle", lua_Form_getStyle},
        {"getTextAlignment", lua_Form_getTextAlignment},
        {"getTextColor", lua_Form_getTextColor},
        {"getTextRightToLeft", lua_Form_getTextRightToLeft},
        {"getTheme", lua_Form_getTheme},
        {"getType", lua_Form_getType},
        {"getWidth", lua_Form_getWidth},
        {"getX", lua_Form_getX},
        {"getY", lua_Form_getY},
        {"getZIndex", lua_Form_getZIndex},
        {"insertControl", lua_Form_insertControl},
        {"isContainer", lua_Form_isContainer},
        {"isEnabled", lua_Form_isEnabled},
        {"isScrollBarsAutoHide", lua_Form_isScrollBarsAutoHide},
        {"isScrolling", lua_Form_isScrolling},
        {"isVisible", lua_Form_isVisible},
        {"release", lua_Form_release},
        {"removeControl", lua_Form_removeControl},
        {"removeListener", lua_Form_removeListener},
        {"removeScriptCallback", lua_Form_removeScriptCallback},
        {"setAlignment", lua_Form_setAlignment},
        {"setAnimationPropertyValue", lua_Form_setAnimationPropertyValue},
        {"setAutoHeight", lua_Form_setAutoHeight},
        {"setAutoWidth", lua_Form_setAutoWidth},
        {"setBorder", lua_Form_setBorder},
        {"setBounds", lua_Form_setBounds},
        {"setConsumeInputEvents", lua_Form_setConsumeInputEvents},
        {"setCursorColor", lua_Form_setCursorColor},
        {"setCursorRegion", lua_Form_setCursorRegion},
        {"setEnabled", lua_Form_setEnabled},
        {"setFocusIndex", lua_Form_setFocusIndex},
        {"setFont", lua_Form_setFont},
        {"setFontSize", lua_Form_setFontSize},
        {"setHeight", lua_Form_setHeight},
        {"setImageColor", lua_Form_setImageColor},
        {"setImageRegion", lua_Form_setImageRegion},
        {"setMargin", lua_Form_setMargin},
        {"setNode", lua_Form_setNode},
        {"setOpacity", lua_Form_setOpacity},
        {"setPadding", lua_Form_setPadding},
        {"setPosition", lua_Form_setPosition},
        {"setScroll", lua_Form_setScroll},
        {"setScrollBarsAutoHide", lua_Form_setScrollBarsAutoHide},
        {"setScrollWheelRequiresFocus", lua_Form_setScrollWheelRequiresFocus},
        {"setScrollWheelSpeed", lua_Form_setScrollWheelSpeed},
        {"setScrollingFriction", lua_Form_setScrollingFriction},
        {"setSize", lua_Form_setSize},
        {"setSkinColor", lua_Form_setSkinColor},
        {"setSkinRegion", lua_Form_setSkinRegion},
        {"setState", lua_Form_setState},
        {"setStyle", lua_Form_setStyle},
        {"setTextAlignment", lua_Form_setTextAlignment},
        {"setTextColor", lua_Form_setTextColor},
        {"setTextRightToLeft", lua_Form_setTextRightToLeft},
        {"setVisible", lua_Form_setVisible},
        {"setWidth", lua_Form_setWidth},
        {"setZIndex", lua_Form_setZIndex},
        {"update", lua_Form_update},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"ANIMATE_OPACITY", lua_Form_static_ANIMATE_OPACITY},
        {"ANIMATE_POSITION", lua_Form_static_ANIMATE_POSITION},
        {"ANIMATE_POSITION_X", lua_Form_static_ANIMATE_POSITION_X},
        {"ANIMATE_POSITION_Y", lua_Form_static_ANIMATE_POSITION_Y},
        {"ANIMATE_SCROLLBAR_OPACITY", lua_Form_static_ANIMATE_SCROLLBAR_OPACITY},
        {"ANIMATE_SIZE", lua_Form_static_ANIMATE_SIZE},
        {"ANIMATE_SIZE_HEIGHT", lua_Form_static_ANIMATE_SIZE_HEIGHT},
        {"ANIMATE_SIZE_WIDTH", lua_Form_static_ANIMATE_SIZE_WIDTH},
        {"create", lua_Form_static_create},
        {"getForm", lua_Form_static_getForm},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Form", lua_members, NULL, lua_Form__gc, lua_statics, scopePath);
}

static Form* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Form");
    luaL_argcheck(state, userdata != NULL, 1, "'Form' expected.");
    return (Form*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Form__gc(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                void* userdata = luaL_checkudata(state, 1, "Form");
                luaL_argcheck(state, userdata != NULL, 1, "'Form' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Form* instance = (Form*)object->instance;
                    SAFE_RELEASE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_Form__gc - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_addControl(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Control> param1 = gameplay::ScriptUtil::getObjectPointer<Control>(2, "Control", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Control'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                unsigned int result = instance->addControl(param1);

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_addControl - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_addListener(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Control::Listener> param1 = gameplay::ScriptUtil::getObjectPointer<Control::Listener>(2, "ControlListener", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Control::Listener'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                Form* instance = getInstance(state);
                instance->addListener(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_addListener - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_addRef(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                instance->addRef();
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_addRef - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_addScriptCallback(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                std::string param1 = gameplay::ScriptUtil::getString(2, true);

                // Get parameter 2 off the stack.
                std::string param2 = gameplay::ScriptUtil::getString(3, true);

                Form* instance = getInstance(state);
                instance->addScriptCallback(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_addScriptCallback - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_createAnimation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    const char* param2 = gameplay::ScriptUtil::getString(3, false);

                    Form* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Properties> param2 = gameplay::ScriptUtil::getObjectPointer<Properties>(3, "Properties", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    Form* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Form_createAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 7:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 7) == LUA_TSTRING || lua_type(state, 7) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                    // Get parameter 4 off the stack.
                    gameplay::ScriptUtil::LuaArray<unsigned int> param4 = gameplay::ScriptUtil::getUnsignedIntPointer(5);

                    // Get parameter 5 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param5 = gameplay::ScriptUtil::getFloatPointer(6);

                    // Get parameter 6 off the stack.
                    Curve::InterpolationType param6 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 7));

                    Form* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2, param3, param4, param5, param6);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Form_createAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 9:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 7) == LUA_TTABLE || lua_type(state, 7) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 8) == LUA_TTABLE || lua_type(state, 8) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 9) == LUA_TSTRING || lua_type(state, 9) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                    // Get parameter 4 off the stack.
                    gameplay::ScriptUtil::LuaArray<unsigned int> param4 = gameplay::ScriptUtil::getUnsignedIntPointer(5);

                    // Get parameter 5 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param5 = gameplay::ScriptUtil::getFloatPointer(6);

                    // Get parameter 6 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param6 = gameplay::ScriptUtil::getFloatPointer(7);

                    // Get parameter 7 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param7 = gameplay::ScriptUtil::getFloatPointer(8);

                    // Get parameter 8 off the stack.
                    Curve::InterpolationType param8 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 9));

                    Form* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2, param3, param4, param5, param6, param7, param8);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Form_createAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3, 7 or 9).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_createAnimationFromBy(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                lua_type(state, 7) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param3 = gameplay::ScriptUtil::getFloatPointer(4);

                // Get parameter 4 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param4 = gameplay::ScriptUtil::getFloatPointer(5);

                // Get parameter 5 off the stack.
                Curve::InterpolationType param5 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                unsigned long param6 = (unsigned long)luaL_checkunsigned(state, 7);

                Form* instance = getInstance(state);
                void* returnPtr = (void*)instance->createAnimationFromBy(param1, param2, param3, param4, param5, param6);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_createAnimationFromBy - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_createAnimationFromTo(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                lua_type(state, 7) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param3 = gameplay::ScriptUtil::getFloatPointer(4);

                // Get parameter 4 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param4 = gameplay::ScriptUtil::getFloatPointer(5);

                // Get parameter 5 off the stack.
                Curve::InterpolationType param5 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                unsigned long param6 = (unsigned long)luaL_checkunsigned(state, 7);

                Form* instance = getInstance(state);
                void* returnPtr = (void*)instance->createAnimationFromTo(param1, param2, param3, param4, param5, param6);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_createAnimationFromTo - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_destroyAnimation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                instance->destroyAnimation();
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_destroyAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                Form* instance = getInstance(state);
                instance->destroyAnimation(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_destroyAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_draw(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                instance->draw();
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_draw - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getAbsoluteBounds(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getAbsoluteBounds());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Rectangle");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getAbsoluteBounds - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getAlignment(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                Control::Alignment result = instance->getAlignment();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_ControlAlignment(result));

                return 1;
            }

            lua_pushstring(state, "lua_Form_getAlignment - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getAnimation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)instance->getAnimation();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                Form* instance = getInstance(state);
                void* returnPtr = (void*)instance->getAnimation(param1);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getAnimationPropertyComponentCount(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                Form* instance = getInstance(state);
                unsigned int result = instance->getAnimationPropertyComponentCount(param1);

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getAnimationPropertyComponentCount - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getAnimationPropertyValue(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<AnimationValue> param2 = gameplay::ScriptUtil::getObjectPointer<AnimationValue>(3, "AnimationValue", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'AnimationValue'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->getAnimationPropertyValue(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_getAnimationPropertyValue - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getAutoHeight(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                bool result = instance->getAutoHeight();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getAutoHeight - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getAutoWidth(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                bool result = instance->getAutoWidth();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getAutoWidth - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getBorder(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getBorder());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "ThemeSideRegions");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getBorder - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getBorder(param1));
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "ThemeSideRegions");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getBorder - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getBounds(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getBounds());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Rectangle");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getBounds - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getClip(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getClip());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Rectangle");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getClip - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getClipBounds(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getClipBounds());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Rectangle");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getClipBounds - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getConsumeInputEvents(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                bool result = instance->getConsumeInputEvents();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getConsumeInputEvents - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getControl(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

                    Form* instance = getInstance(state);
                    void* returnPtr = (void*)instance->getControl(param1);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Control");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    Form* instance = getInstance(state);
                    void* returnPtr = (void*)instance->getControl(param1);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Control");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Form_getControl - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getCursorColor(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getCursorColor(param1));
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Vector4");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getCursorColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getCursorRegion(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getCursorRegion(param1));
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Rectangle");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getCursorRegion - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getCursorUVs(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getCursorUVs(param1));
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "ThemeUVs");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getCursorUVs - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getFocusIndex(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                int result = instance->getFocusIndex();

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getFocusIndex - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getFont(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)instance->getFont();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Font");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getFont - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                void* returnPtr = (void*)instance->getFont(param1);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Font");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getFont - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getFontSize(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                unsigned int result = instance->getFontSize();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getFontSize - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                unsigned int result = instance->getFontSize(param1);

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getFontSize - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getHeight(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                float result = instance->getHeight();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getHeight - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getId(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                const char* result = instance->getId();

                // Push the return value onto the stack.
                lua_pushstring(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getId - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getImageColor(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                Control::State param2 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 3));

                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getImageColor(param1, param2));
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Vector4");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getImageColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getImageRegion(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                Control::State param2 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 3));

                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getImageRegion(param1, param2));
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Rectangle");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getImageRegion - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getImageUVs(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                Control::State param2 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 3));

                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getImageUVs(param1, param2));
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "ThemeUVs");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getImageUVs - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getLayout(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)instance->getLayout();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Layout");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getLayout - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getMargin(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getMargin());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "ThemeSideRegions");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getMargin - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getOpacity(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                float result = instance->getOpacity();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getOpacity - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                float result = instance->getOpacity(param1);

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getOpacity - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getPadding(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getPadding());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "ThemeSideRegions");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getPadding - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getRefCount(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                unsigned int result = instance->getRefCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getRefCount - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getScroll(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                Container::Scroll result = instance->getScroll();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_ContainerScroll(result));

                return 1;
            }

            lua_pushstring(state, "lua_Form_getScroll - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getScrollWheelRequiresFocus(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                bool result = instance->getScrollWheelRequiresFocus();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getScrollWheelRequiresFocus - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getScrollWheelSpeed(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                float result = instance->getScrollWheelSpeed();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getScrollWheelSpeed - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getScrollingFriction(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                float result = instance->getScrollingFriction();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getScrollingFriction - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getSkinColor(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getSkinColor());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Vector4");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getSkinColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getSkinColor(param1));
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Vector4");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getSkinColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getSkinRegion(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getSkinRegion());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Rectangle");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getSkinRegion - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getSkinRegion(param1));
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Rectangle");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getSkinRegion - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getState(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                Control::State result = instance->getState();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_ControlState(result));

                return 1;
            }

            lua_pushstring(state, "lua_Form_getState - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getStyle(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)instance->getStyle();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "ThemeStyle");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getStyle - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getTextAlignment(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                Font::Justify result = instance->getTextAlignment();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_FontJustify(result));

                return 1;
            }

            lua_pushstring(state, "lua_Form_getTextAlignment - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                Font::Justify result = instance->getTextAlignment(param1);

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_FontJustify(result));

                return 1;
            }

            lua_pushstring(state, "lua_Form_getTextAlignment - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getTextColor(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getTextColor());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Vector4");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getTextColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getTextColor(param1));
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Vector4");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getTextColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getTextRightToLeft(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                bool result = instance->getTextRightToLeft();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getTextRightToLeft - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                bool result = instance->getTextRightToLeft(param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getTextRightToLeft - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getTheme(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                void* returnPtr = (void*)instance->getTheme();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Theme");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_getTheme - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getType(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                const char* result = instance->getType();

                // Push the return value onto the stack.
                lua_pushstring(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getType - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getWidth(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                float result = instance->getWidth();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getWidth - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getX(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                float result = instance->getX();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getX - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getY(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                float result = instance->getY();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getY - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_getZIndex(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                int result = instance->getZIndex();

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_getZIndex - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_insertControl(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Control> param1 = gameplay::ScriptUtil::getObjectPointer<Control>(2, "Control", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Control'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                Form* instance = getInstance(state);
                instance->insertControl(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_insertControl - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_isContainer(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                bool result = instance->isContainer();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_isContainer - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_isEnabled(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                bool result = instance->isEnabled();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_isEnabled - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_isScrollBarsAutoHide(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                bool result = instance->isScrollBarsAutoHide();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_isScrollBarsAutoHide - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_isScrolling(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                bool result = instance->isScrolling();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_isScrolling - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_isVisible(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                bool result = instance->isVisible();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Form_isVisible - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_release(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Form* instance = getInstance(state);
                instance->release();
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_release - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_removeControl(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

                    Form* instance = getInstance(state);
                    instance->removeControl(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    Form* instance = getInstance(state);
                    instance->removeControl(param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Control> param1 = gameplay::ScriptUtil::getObjectPointer<Control>(2, "Control", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    Form* instance = getInstance(state);
                    instance->removeControl(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Form_removeControl - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_removeListener(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Control::Listener> param1 = gameplay::ScriptUtil::getObjectPointer<Control::Listener>(2, "ControlListener", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Control::Listener'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->removeListener(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_removeListener - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_removeScriptCallback(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                std::string param1 = gameplay::ScriptUtil::getString(2, true);

                // Get parameter 2 off the stack.
                std::string param2 = gameplay::ScriptUtil::getString(3, true);

                Form* instance = getInstance(state);
                instance->removeScriptCallback(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_removeScriptCallback - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setAlignment(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::Alignment param1 = (Control::Alignment)lua_enumFromString_ControlAlignment(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                instance->setAlignment(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setAlignment - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setAnimationPropertyValue(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<AnimationValue> param2 = gameplay::ScriptUtil::getObjectPointer<AnimationValue>(3, "AnimationValue", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'AnimationValue'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->setAnimationPropertyValue(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setAnimationPropertyValue - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<AnimationValue> param2 = gameplay::ScriptUtil::getObjectPointer<AnimationValue>(3, "AnimationValue", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'AnimationValue'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                Form* instance = getInstance(state);
                instance->setAnimationPropertyValue(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setAnimationPropertyValue - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setAutoHeight(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                Form* instance = getInstance(state);
                instance->setAutoHeight(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setAutoHeight - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setAutoWidth(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                Form* instance = getInstance(state);
                instance->setAutoWidth(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setAutoWidth - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setBorder(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 5);

                Form* instance = getInstance(state);
                instance->setBorder(param1, param2, param3, param4);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setBorder - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 6:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 5);

                // Get parameter 5 off the stack.
                unsigned char param5 = (unsigned char)luaL_checkunsigned(state, 6);

                Form* instance = getInstance(state);
                instance->setBorder(param1, param2, param3, param4, param5);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setBorder - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 5 or 6).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setBounds(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Rectangle'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->setBounds(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setBounds - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setConsumeInputEvents(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                Form* instance = getInstance(state);
                instance->setConsumeInputEvents(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setConsumeInputEvents - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setCursorColor(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector4'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned char param2 = (unsigned char)luaL_checkunsigned(state, 3);

                Form* instance = getInstance(state);
                instance->setCursorColor(*param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setCursorColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setCursorRegion(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned char param2 = (unsigned char)luaL_checkunsigned(state, 3);

                Form* instance = getInstance(state);
                instance->setCursorRegion(*param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setCursorRegion - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setEnabled(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                Form* instance = getInstance(state);
                instance->setEnabled(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setEnabled - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setFocusIndex(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                Form* instance = getInstance(state);
                instance->setFocusIndex(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setFocusIndex - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setFont(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Font> param1 = gameplay::ScriptUtil::getObjectPointer<Font>(2, "Font", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Font'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->setFont(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setFont - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Font> param1 = gameplay::ScriptUtil::getObjectPointer<Font>(2, "Font", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Font'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned char param2 = (unsigned char)luaL_checkunsigned(state, 3);

                Form* instance = getInstance(state);
                instance->setFont(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setFont - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setFontSize(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

                Form* instance = getInstance(state);
                instance->setFontSize(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setFontSize - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

                // Get parameter 2 off the stack.
                unsigned char param2 = (unsigned char)luaL_checkunsigned(state, 3);

                Form* instance = getInstance(state);
                instance->setFontSize(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setFontSize - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setHeight(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Form* instance = getInstance(state);
                instance->setHeight(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setHeight - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setImageColor(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param2 = gameplay::ScriptUtil::getObjectPointer<Vector4>(3, "Vector4", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Vector4'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->setImageColor(param1, *param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setImageColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param2 = gameplay::ScriptUtil::getObjectPointer<Vector4>(3, "Vector4", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Vector4'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                unsigned char param3 = (unsigned char)luaL_checkunsigned(state, 4);

                Form* instance = getInstance(state);
                instance->setImageColor(param1, *param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setImageColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setImageRegion(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->setImageRegion(param1, *param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setImageRegion - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                unsigned char param3 = (unsigned char)luaL_checkunsigned(state, 4);

                Form* instance = getInstance(state);
                instance->setImageRegion(param1, *param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setImageRegion - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setMargin(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 5);

                Form* instance = getInstance(state);
                instance->setMargin(param1, param2, param3, param4);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setMargin - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setNode(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Node> param1 = gameplay::ScriptUtil::getObjectPointer<Node>(2, "Node", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Node'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->setNode(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setNode - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setOpacity(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Form* instance = getInstance(state);
                instance->setOpacity(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setOpacity - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                unsigned char param2 = (unsigned char)luaL_checkunsigned(state, 3);

                Form* instance = getInstance(state);
                instance->setOpacity(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setOpacity - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setPadding(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 5);

                Form* instance = getInstance(state);
                instance->setPadding(param1, param2, param3, param4);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setPadding - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setPosition(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                Form* instance = getInstance(state);
                instance->setPosition(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setPosition - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setScroll(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Container::Scroll param1 = (Container::Scroll)lua_enumFromString_ContainerScroll(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                instance->setScroll(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setScroll - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setScrollBarsAutoHide(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                Form* instance = getInstance(state);
                instance->setScrollBarsAutoHide(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setScrollBarsAutoHide - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setScrollWheelRequiresFocus(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                Form* instance = getInstance(state);
                instance->setScrollWheelRequiresFocus(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setScrollWheelRequiresFocus - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setScrollWheelSpeed(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Form* instance = getInstance(state);
                instance->setScrollWheelSpeed(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setScrollWheelSpeed - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setScrollingFriction(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Form* instance = getInstance(state);
                instance->setScrollingFriction(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setScrollingFriction - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setSize(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                Form* instance = getInstance(state);
                instance->setSize(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setSize - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setSkinColor(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector4'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->setSkinColor(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setSkinColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector4'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned char param2 = (unsigned char)luaL_checkunsigned(state, 3);

                Form* instance = getInstance(state);
                instance->setSkinColor(*param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setSkinColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setSkinRegion(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Rectangle'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->setSkinRegion(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setSkinRegion - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned char param2 = (unsigned char)luaL_checkunsigned(state, 3);

                Form* instance = getInstance(state);
                instance->setSkinRegion(*param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setSkinRegion - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setState(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Control::State param1 = (Control::State)lua_enumFromString_ControlState(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                instance->setState(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setState - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setStyle(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Theme::Style> param1 = gameplay::ScriptUtil::getObjectPointer<Theme::Style>(2, "ThemeStyle", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Theme::Style'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->setStyle(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setStyle - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setTextAlignment(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Font::Justify param1 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 2));

                Form* instance = getInstance(state);
                instance->setTextAlignment(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setTextAlignment - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                Font::Justify param1 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 2));

                // Get parameter 2 off the stack.
                unsigned char param2 = (unsigned char)luaL_checkunsigned(state, 3);

                Form* instance = getInstance(state);
                instance->setTextAlignment(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setTextAlignment - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setTextColor(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector4'.");
                    lua_error(state);
                }

                Form* instance = getInstance(state);
                instance->setTextColor(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setTextColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector4'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned char param2 = (unsigned char)luaL_checkunsigned(state, 3);

                Form* instance = getInstance(state);
                instance->setTextColor(*param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setTextColor - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setTextRightToLeft(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                Form* instance = getInstance(state);
                instance->setTextRightToLeft(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setTextRightToLeft - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                // Get parameter 2 off the stack.
                unsigned char param2 = (unsigned char)luaL_checkunsigned(state, 3);

                Form* instance = getInstance(state);
                instance->setTextRightToLeft(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setTextRightToLeft - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setVisible(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                Form* instance = getInstance(state);
                instance->setVisible(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setVisible - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setWidth(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Form* instance = getInstance(state);
                instance->setWidth(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setWidth - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_setZIndex(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                Form* instance = getInstance(state);
                instance->setZIndex(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_setZIndex - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_static_ANIMATE_OPACITY(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Form::ANIMATE_OPACITY;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Form_static_ANIMATE_POSITION(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Form::ANIMATE_POSITION;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Form_static_ANIMATE_POSITION_X(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Form::ANIMATE_POSITION_X;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Form_static_ANIMATE_POSITION_Y(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Form::ANIMATE_POSITION_Y;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Form_static_ANIMATE_SCROLLBAR_OPACITY(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Form::ANIMATE_SCROLLBAR_OPACITY;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Form_static_ANIMATE_SIZE(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Form::ANIMATE_SIZE;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Form_static_ANIMATE_SIZE_HEIGHT(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Form::ANIMATE_SIZE_HEIGHT;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Form_static_ANIMATE_SIZE_WIDTH(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Form::ANIMATE_SIZE_WIDTH;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Form_static_create(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(1, false);

                    void* returnPtr = (void*)Form::create(param1);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "Form");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Form_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(1, false);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Theme::Style> param2 = gameplay::ScriptUtil::getObjectPointer<Theme::Style>(2, "ThemeStyle", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    void* returnPtr = (void*)Form::create(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "Form");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Form_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(1, false);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Theme::Style> param2 = gameplay::ScriptUtil::getObjectPointer<Theme::Style>(2, "ThemeStyle", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    Layout::Type param3 = (Layout::Type)lua_enumFromString_LayoutType(luaL_checkstring(state, 3));

                    void* returnPtr = (void*)Form::create(param1, param2, param3);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "Form");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Form_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1, 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_static_getForm(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(1, false);

                void* returnPtr = (void*)Form::getForm(param1);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Form");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Form_static_getForm - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Form_update(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                Form* instance = getInstance(state);
                instance->update(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Form_update - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
