#include "Base.h"
#include "lua_PhysicsControllerListenerEventType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_PhysicsControllerListenerEventType_ACTIVATED = "ACTIVATED";
static const char* luaEnumString_PhysicsControllerListenerEventType_DEACTIVATED = "DEACTIVATED";

PhysicsController::Listener::EventType lua_enumFromString_PhysicsControllerListenerEventType(const char* s)
{
    if (strcmp(s, luaEnumString_PhysicsControllerListenerEventType_ACTIVATED) == 0)
        return PhysicsController::Listener::ACTIVATED;
    if (strcmp(s, luaEnumString_PhysicsControllerListenerEventType_DEACTIVATED) == 0)
        return PhysicsController::Listener::DEACTIVATED;
    GP_ERROR("Invalid enumeration value '%s' for enumeration PhysicsController::Listener::EventType.", s);
    return PhysicsController::Listener::ACTIVATED;
}

const char* lua_stringFromEnum_PhysicsControllerListenerEventType(PhysicsController::Listener::EventType e)
{
    if (e == PhysicsController::Listener::ACTIVATED)
        return luaEnumString_PhysicsControllerListenerEventType_ACTIVATED;
    if (e == PhysicsController::Listener::DEACTIVATED)
        return luaEnumString_PhysicsControllerListenerEventType_DEACTIVATED;
    GP_ERROR("Invalid enumeration value '%d' for enumeration PhysicsController::Listener::EventType.", e);
    return enumStringEmpty;
}

}

