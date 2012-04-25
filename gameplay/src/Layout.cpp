#include "Base.h"
#include "Layout.h"
#include "Control.h"
#include "Container.h"

namespace gameplay
{
    void Layout::align(Control* control, const Container* container)
    {
        if (control->_alignment != Control::ALIGN_TOP_LEFT ||
            control->_autoWidth || control->_autoHeight)
        {
            Rectangle controlBounds = control->getBounds();
            const Rectangle& containerBounds = container->getBounds();
            const Theme::Border& containerBorder = container->getBorder(container->getState());
            const Theme::Padding& containerPadding = container->getPadding();

            float clipWidth = containerBounds.width - containerBorder.left - containerBorder.right - containerPadding.left - containerPadding.right;
            float clipHeight = containerBounds.height - containerBorder.top - containerBorder.bottom - containerPadding.top - containerPadding.bottom;

            if (control->_autoWidth)
            {
                controlBounds.width = clipWidth;
            }

            if (control->_autoHeight)
            {
                controlBounds.height = clipHeight;
            }

            // Vertical alignment
            if ((control->_alignment & Control::ALIGN_BOTTOM) == Control::ALIGN_BOTTOM)
            {
                controlBounds.y = clipHeight - controlBounds.height;
            }
            else if ((control->_alignment & Control::ALIGN_VCENTER) == Control::ALIGN_VCENTER)
            {
                controlBounds.y = clipHeight * 0.5f - controlBounds.height * 0.5f;
            }

            // Horizontal alignment
            if ((control->_alignment & Control::ALIGN_RIGHT) == Control::ALIGN_RIGHT)
            {
                controlBounds.x = clipWidth - controlBounds.width;
            }
            else if ((control->_alignment & Control::ALIGN_HCENTER) == Control::ALIGN_HCENTER)
            {
                controlBounds.x = clipWidth * 0.5f - controlBounds.width * 0.5f;
            }

            control->setBounds(controlBounds);
        }
    }

    bool Layout::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
    {
        return false;
    }
}