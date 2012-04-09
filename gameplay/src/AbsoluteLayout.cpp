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

                // Vertical alignment
                if ((control->_alignment & Control::ALIGN_BOTTOM) == Control::ALIGN_BOTTOM)
                {
                    controlBounds.y = containerBounds.height - controlBounds.height;
                }
                else if ((control->_alignment & Control::ALIGN_VCENTER) == Control::ALIGN_VCENTER)
                {
                    controlBounds.y = containerBounds.height / 2.0f - controlBounds.height / 2.0f;
                }

                // Horizontal alignment
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

            if (control->isDirty())
            {
                control->update(container->getClip());
            }
        }
    }
}