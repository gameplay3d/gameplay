#include "Base.h"
#include "DAEChannelTarget.h"


namespace gameplay
{

DAEChannelTarget::DAEChannelTarget(const domChannelRef channelRef) : _channel(channelRef), _targetElement(NULL)
{
    const std::string target = channelRef->getTarget();
    size_t index = target.find('/');
    if (index == std::string::npos)
    {
        // If the string doesn't contain a '/' then the whole string is the id
        // and there are no sid's being targeted.
        _targetId = target;
    }
    else
    {
        // The targetId is the part before the first '/'
        _targetId = target.substr(0, index);

        // each '/' denotes another sid
        size_t start;
        size_t end;
        do
        {
            start = index + 1;
            end = target.find('/', start);
        
            std::string sub;
            if (end == std::string::npos)
            {
                sub = target.substr(start);
                // break;
            }
            else
            {
                sub = target.substr(start, end - start);
                index = end + 1;
            }
            _attributeIds.push_back(sub);
        } while(end != std::string::npos);
    }

}

DAEChannelTarget::~DAEChannelTarget(void)
{
}

daeElement* DAEChannelTarget::getTargetElement()
{
    if (!_targetElement && _targetId.length() > 0)
    {
        daeSIDResolver resolver(_channel->getDocument()->getDomRoot(), _targetId.c_str());
        _targetElement = resolver.getElement();
    }
    return _targetElement;
}

const std::string& DAEChannelTarget::getTargetId() const
{
    return _targetId;
}

size_t DAEChannelTarget::getTargetAttributeCount() const
{
    return _attributeIds.size();
}

daeElement* DAEChannelTarget::getTargetAttribute(size_t index)
{
    if (index >= _attributeIds.size())
    {
        return NULL;
    }
    const std::string& att = _attributeIds[index];
    std::string sid = att.substr(0, att.find('.'));
    daeSIDResolver resolver(getTargetElement(), sid.c_str());
    return resolver.getElement();
}

void DAEChannelTarget::getPropertyName(size_t index, std::string* str)
{
    if (index < _attributeIds.size())
    {
        // The property name is the string segment after the '.'
        // The propery is optional so it might not be found.
        const std::string& att = _attributeIds[index];
        size_t i = att.find('.');
        if (i != std::string::npos && i < att.size())
        {
            str->assign(att.substr(i+1));
            return;
        }
    }
    str->clear();
}

}
