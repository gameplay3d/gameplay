#include "Base.h"
#include "VerticalLayout.h"

namespace gameplay
{
    static VerticalLayout* __instance;

    VerticalLayout::VerticalLayout() : _bottomToTop(false)
    {
    }

    VerticalLayout::VerticalLayout(const VerticalLayout& copy)
    {
    }

    VerticalLayout::~VerticalLayout()
    {
    }

    VerticalLayout* VerticalLayout::create()
    {
        if (!__instance)
        {
            __instance = new VerticalLayout();
        }
        else
        {
            __instance->addRef();
        }

        return __instance;
    }

    void VerticalLayout::setBottomToTop(bool bottomToTop)
    {
        _bottomToTop = bottomToTop;
    }

    Layout::Type VerticalLayout::getType()
    {
        return Layout::LAYOUT_VERTICAL;
    }

    void VerticalLayout::update(const Container* container)
    {
        // Need border, padding.
        Theme::Border border = container->getBorder(container->getState());
        Theme::Padding padding = container->getPadding();

        float yPosition = 0;

        std::vector<Control*> controls = container->getControls();

        unsigned int i, end, iter;
        if (_bottomToTop)
        {
            i = controls.size() - 1;
            end = -1;
            iter = -1;
        }
        else
        {
            i = 0;
            end = controls.size();
            iter = 1;
        }

        while (i != end)
        {
            Control* control = controls.at(i);
            
            align(control, container);

            const Rectangle& bounds = control->getBounds();
            const Theme::Margin& margin = control->getMargin();

            yPosition += margin.top;

            if (control->isDirty() || control->isContainer())
            {
                control->setPosition(0, yPosition);
                control->update(container->getClip(), Vector2::zero());
            }

            yPosition += bounds.height + margin.bottom;

            i += iter;
        }
    }
}