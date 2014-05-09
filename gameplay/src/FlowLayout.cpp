#include "Base.h"
#include "Control.h"
#include "FlowLayout.h"
#include "Container.h"

namespace gameplay
{

static FlowLayout* __instance;

FlowLayout::FlowLayout() : _horizontalSpacing(0), _verticalSpacing(0)
{
}

FlowLayout::~FlowLayout()
{
    __instance = NULL;
}

FlowLayout* FlowLayout::create()
{
    if (!__instance)
    {
        __instance = new FlowLayout();
    }
    else
    {
        __instance->addRef();
    }

    return __instance;
}

Layout::Type FlowLayout::getType()
{
    return Layout::LAYOUT_FLOW;
}

int FlowLayout::getHorizontalSpacing() const
{
    return _horizontalSpacing;
}

int FlowLayout::getVerticalSpacing() const
{
    return _verticalSpacing;
}

void FlowLayout::setSpacing(int horizontalSpacing, int verticalSpacing)
{
    _horizontalSpacing = horizontalSpacing;
    _verticalSpacing = verticalSpacing;
}

void FlowLayout::update(const Container* container)
{
    GP_ASSERT(container);
    const Rectangle& containerBounds = container->getBounds();
    const Theme::Border& containerBorder = container->getBorder(container->getState());
    const Theme::Padding& containerPadding = container->getPadding();

    float clipWidth = containerBounds.width - containerBorder.left - containerBorder.right - containerPadding.left - containerPadding.right;
    float clipHeight = containerBounds.height - containerBorder.top - containerBorder.bottom - containerPadding.top - containerPadding.bottom;

    float xPosition = 0;
    float yPosition = 0;
    float rowY = 0;
    float tallestHeight = 0;

    std::vector<Control*> controls = container->getControls();
    for (size_t i = 0, controlsCount = controls.size(); i < controlsCount; i++)
    {
        Control* control = controls.at(i);
        GP_ASSERT(control);

        if (!control->isVisible())
            continue;

        const Rectangle& bounds = control->getBounds();
        const Theme::Margin& margin = control->getMargin();

        xPosition += margin.left;

        // Wrap to next row if we've gone past the edge of the container.
        if (xPosition + bounds.width >= clipWidth)
        {
            xPosition = margin.left;
            rowY += tallestHeight + _verticalSpacing;
            tallestHeight = 0;
        }

        yPosition = rowY + margin.top;

        control->setPosition(xPosition, yPosition);

        xPosition += bounds.width + margin.right + _horizontalSpacing;

        float height = bounds.height + margin.top + margin.bottom;
        if (height > tallestHeight)
        {
            tallestHeight = height;
        }
    }
}

}