#include "Base.h"
#include "Control.h"
#include "AbsoluteLayout.h"
#include "Container.h"

namespace gameplay
{

static AbsoluteLayout* __instance;

AbsoluteLayout::AbsoluteLayout()
{
}

AbsoluteLayout::AbsoluteLayout(const AbsoluteLayout& copy)
{
}

AbsoluteLayout::~AbsoluteLayout()
{
    __instance = NULL;
}

AbsoluteLayout* AbsoluteLayout::create()
{
    if (!__instance)
    {
        __instance = new AbsoluteLayout();
    }
    else
    {
        __instance->addRef();
    }

    return __instance;
}

Layout::Type AbsoluteLayout::getType()
{
    return Layout::LAYOUT_ABSOLUTE;
}

void AbsoluteLayout::update(const Container* container)
{
    // An AbsoluteLayout does nothing to modify the layout of Controls.
    std::vector<Control*> controls = container->getControls();
    unsigned int controlsCount = controls.size();
    for (unsigned int i = 0; i < controlsCount; i++)
    {
        Control* control = controls[i];

        if (control->isDirty() || control->isContainer())
        {
            align(control, container);
            control->update(container->getClip(), Vector2::zero());
        }
    }
}

}