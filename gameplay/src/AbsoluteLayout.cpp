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

void AbsoluteLayout::update(const Container* container, const Vector2& offset)
{
    GP_ASSERT(container);

    // An AbsoluteLayout does nothing to modify the layout of Controls.
    const std::vector<Control*>& controls = container->getControls();
    for (size_t i = 0, count = controls.size(); i < count; i++)
    {
        Control* control = controls[i];
        GP_ASSERT(control);

        align(control, container);
        control->update(container, offset);
    }
}

}