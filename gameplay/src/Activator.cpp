#include "Base.h"
#include "Activator.h"

namespace gameplay
{

Activator::Activator()
{
}

Activator::~Activator()
{
}

Activator* Activator::getActivator()
{
    static Activator activator;
    return &activator;
}
       
std::shared_ptr<Serializable> Activator::createObject(const std::string& className)
{
    std::shared_ptr<Serializable> object = nullptr;
    std::map<std::string, CreateObjectCallback>::const_iterator itr = _classes.find(className);
    if (itr == _classes.end())
    {
        return nullptr;
    }
    
    CreateObjectCallback createObject = itr->second;
    if (createObject)
    {
        object = createObject();
    }
    return object;
}
    
std::string Activator::enumToString(const std::string& enumName, int value)
{
    std::map<std::string,std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = _enums.find(enumName);
    if (itr != _enums.end())
    {
        EnumToStringCallback enumToString = itr->second.first;
        if (enumToString)
        {
            return enumToString(enumName, value);
        }
    }
    return nullptr;
}

int Activator::enumParse(const std::string& enumName, const std::string& str)
{
    std::map<std::string,std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = _enums.find(enumName);
    if (itr != _enums.end())
    {
        EnumParseCallback enumParse = itr->second.second;
        if (enumParse)
        {
            return enumParse(enumName, str);
        }
    }
    return 0;
}

void Activator::registerType(const std::string&  className, CreateObjectCallback createObject)
{
    std::map<std::string,CreateObjectCallback>::const_iterator itr = _classes.find(className);
    if ( itr == _classes.end() )
    {
        _classes[className] = createObject;
    }
    else
    {
        GP_ERROR("className already registered:%s", className.c_str());
    }
}
    
void Activator::registerEnum(const std::string& enumName, EnumToStringCallback toString, EnumParseCallback parse)
{
    std::map<std::string, std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = _enums.find(enumName);
    if (itr == _enums.end())
    {
        _enums[enumName] = std::make_pair(toString, parse);
    }
    else
    {
        GP_ERROR("enumName already registered:%s", enumName.c_str());
    }
}

}
