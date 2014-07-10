#include "Base.h"
#include "AIMessage.h"

namespace gameplay
{

AIMessage::AIMessage()
    : _id(0), _deliveryTime(0), _parameters(NULL), _parameterCount(0), _messageType(MESSAGE_TYPE_CUSTOM), _next(NULL)
{
}

AIMessage::~AIMessage()
{
    SAFE_DELETE_ARRAY(_parameters);
}

AIMessage* AIMessage::create(unsigned int id, const char* sender, const char* receiver, unsigned int parameterCount)
{
    AIMessage* message = new AIMessage();
    message->_id = id;
    message->_sender = sender;
    message->_receiver = receiver;
    message->_parameterCount = parameterCount;
    if (parameterCount > 0)
        message->_parameters = new AIMessage::Parameter[parameterCount];
    return message;
}

void AIMessage::destroy(AIMessage* message)
{
    SAFE_DELETE(message);
}

unsigned int AIMessage::getId() const
{
    return _id;
}

const char* AIMessage::getSender() const
{
    return _sender.c_str();
}

const char* AIMessage::getReceiver() const
{
    return _receiver.c_str();
}

double AIMessage::getDeliveryTime() const
{
    return _deliveryTime;
}

int AIMessage::getInt(unsigned int index) const
{
    GP_ASSERT(index < _parameterCount);
    GP_ASSERT(_parameters[index].type == AIMessage::INTEGER);

    return _parameters[index].intValue;
}

void AIMessage::setInt(unsigned int index, int value)
{
    GP_ASSERT(index < _parameterCount);

    clearParameter(index);

    _parameters[index].intValue = value;
    _parameters[index].type = AIMessage::INTEGER;
}

long AIMessage::getLong(unsigned int index) const
{
    GP_ASSERT(index < _parameterCount);
    GP_ASSERT(_parameters[index].type == AIMessage::LONG);

    return _parameters[index].longValue;
}

void AIMessage::setLong(unsigned int index, long value)
{
    GP_ASSERT(index < _parameterCount);

    clearParameter(index);

    _parameters[index].longValue = value;
    _parameters[index].type = AIMessage::LONG;
}

float AIMessage::getFloat(unsigned int index) const
{
    GP_ASSERT(index < _parameterCount);
    GP_ASSERT(_parameters[index].type == AIMessage::FLOAT);

    return _parameters[index].floatValue;
}

void AIMessage::setFloat(unsigned int index, float value)
{
    GP_ASSERT(index < _parameterCount);

    clearParameter(index);

    _parameters[index].floatValue = value;
    _parameters[index].type = AIMessage::FLOAT;
}

double AIMessage::getDouble(unsigned int index) const
{
    GP_ASSERT(index < _parameterCount);
    GP_ASSERT(_parameters[index].type == AIMessage::DOUBLE);

    return _parameters[index].doubleValue;
}

void AIMessage::setDouble(unsigned int index, double value)
{
    GP_ASSERT(index < _parameterCount);

    clearParameter(index);

    _parameters[index].doubleValue = value;
    _parameters[index].type = AIMessage::DOUBLE;
}

bool AIMessage::getBoolean(unsigned int index) const
{
    GP_ASSERT(index < _parameterCount);
    GP_ASSERT(_parameters[index].type == AIMessage::BOOLEAN);

    return _parameters[index].boolValue;
}

void AIMessage::setBoolean(unsigned int index, bool value)
{
    GP_ASSERT(index < _parameterCount);

    clearParameter(index);

    _parameters[index].boolValue = value;
    _parameters[index].type = AIMessage::BOOLEAN;
}

const char* AIMessage::getString(unsigned int index) const
{
    GP_ASSERT(index < _parameterCount);
    GP_ASSERT(_parameters[index].type == AIMessage::STRING);

    return _parameters[index].stringValue;
}

void AIMessage::setString(unsigned int index, const char* value)
{
    GP_ASSERT(index < _parameterCount);
    GP_ASSERT(value);

    clearParameter(index);

    // Copy the string into our parameter
    size_t len = strlen(value);
    char* buffer = new char[len + 1];
    strcpy(buffer, value);
    _parameters[index].stringValue = buffer;
    _parameters[index].type = AIMessage::STRING;
}

unsigned int AIMessage::getParameterCount() const
{
    return _parameterCount;
}

AIMessage::ParameterType AIMessage::getParameterType(unsigned int index) const
{
    GP_ASSERT(index < _parameterCount);

    return _parameters[index].type;
}

void AIMessage::clearParameter(unsigned int index)
{
    GP_ASSERT(index < _parameterCount);

    _parameters[index].clear();
}

AIMessage::Parameter::Parameter()
    : type(UNDEFINED)
{
}

AIMessage::Parameter::~Parameter()
{
    clear();
}

void AIMessage::Parameter::clear()
{
    if (type == AIMessage::STRING)
        SAFE_DELETE_ARRAY(stringValue);

    type = AIMessage::UNDEFINED;
}

}
