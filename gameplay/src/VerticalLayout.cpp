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

            if (control->_alignment != Control::ALIGN_TOP_LEFT ||
                control->_autoWidth || control->_autoHeight)
            {
                Rectangle controlBounds = control->getBounds();
                const Rectangle& containerBounds = container->getBounds();

                if (control->_autoWidth)
                {
                    controlBounds.width = containerBounds.width;
                }

                if (control->_autoHeight)
                {
                    controlBounds.height = containerBounds.height;
                }

                if ((control->_alignment & Control::ALIGN_RIGHT) == Control::ALIGN_RIGHT)
                {
                    controlBounds.x = containerBounds.width - controlBounds.width;
                }
                else if ((control->_alignment & Control::ALIGN_HCENTER) == Control::ALIGN_HCENTER)
                {
                    controlBounds.x = containerBounds.width / 2.0f - controlBounds.width / 2.0f;
                }

                control->setBounds(controlBounds);
            }

            const Rectangle& bounds = control->getClipBounds();
            const Theme::Margin& margin = control->getMargin();

            yPosition += margin.top;

            control->setPosition(0, yPosition);
            if (control->isDirty())
            {
                control->update(container->getClip());
            }

            yPosition += bounds.height + margin.bottom;

            i += iter;
        }
    }
}