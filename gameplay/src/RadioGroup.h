#ifndef RADIOGROUP_H_
#define RADIOGROUP_H_

#include "Container.h"
#include "Theme.h"
#include "Properties.h"

namespace gameplay
{

class RadioGroup : public Container
{
public:
    RadioGroup();
    virtual ~RadioGroup();

    static RadioGroup* create(Theme::Style* style, Properties* properties);
    static RadioGroup* getRadioGroup(const char* id);

private:
    RadioGroup(const RadioGroup& copy);

    unsigned int _selectedIndex;
    std::vector<RadioButton*> _choices;
}

}

#endif