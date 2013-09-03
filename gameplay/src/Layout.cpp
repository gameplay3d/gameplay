#include "Base.h"
#include "Layout.h"
#include "Control.h"
#include "Container.h"

namespace gameplay
{

void Layout::align(Control* control, const Container* container)
{
    GP_ASSERT(control);
    GP_ASSERT(container);

    if (control->_alignment != Control::ALIGN_TOP_LEFT ||
        control->_isAlignmentSet ||
        control->_autoWidth || control->_autoHeight)
    {
        Rectangle controlBounds = control->getBounds();
        const Theme::Margin& controlMargin = control->getMargin();
        const Rectangle& containerBounds = container->getBounds();
        const Theme::Border& containerBorder = container->getBorder(container->getState());
        const Theme::Padding& containerPadding = container->getPadding();

        float clipWidth;
        float clipHeight; 
        if (container->getScroll() != Container::SCROLL_NONE)
        {
            const Rectangle& verticalScrollBarBounds = container->getImageRegion("verticalScrollBar", container->getState());
            const Rectangle& horizontalScrollBarBounds = container->getImageRegion("horizontalScrollBar", container->getState());
            clipWidth = containerBounds.width - containerBorder.left - containerBorder.right - containerPadding.left - containerPadding.right - verticalScrollBarBounds.width;
            clipHeight = containerBounds.height - containerBorder.top - containerBorder.bottom - containerPadding.top - containerPadding.bottom - horizontalScrollBarBounds.height;
        }
        else
        {
            clipWidth = containerBounds.width - containerBorder.left - containerBorder.right - containerPadding.left - containerPadding.right;
            clipHeight = containerBounds.height - containerBorder.top - containerBorder.bottom - containerPadding.top - containerPadding.bottom;
        }

        if (control->_autoWidth)
        {
            controlBounds.width = clipWidth - controlMargin.left - controlMargin.right;
        }

        if (control->_autoHeight)
        {
            controlBounds.height = clipHeight - controlMargin.top - controlMargin.bottom;
        }

        // Vertical alignment
        if(control->_isAlignmentSet || control->_autoHeight)
        {
            if ((control->_alignment & Control::ALIGN_BOTTOM) == Control::ALIGN_BOTTOM)
            {
                controlBounds.y = clipHeight - controlBounds.height - controlMargin.bottom;
            }
            else if ((control->_alignment & Control::ALIGN_VCENTER) == Control::ALIGN_VCENTER)
            {
                controlBounds.y = clipHeight * 0.5f - controlBounds.height * 0.5f;
            }
            else if ((control->_alignment & Control::ALIGN_TOP) == Control::ALIGN_TOP)
            {
                controlBounds.y = controlMargin.top;
            }
        }

        // Horizontal alignment
        if(control->_isAlignmentSet || control->_autoWidth)
        {
            if ((control->_alignment & Control::ALIGN_RIGHT) == Control::ALIGN_RIGHT)
            {
                controlBounds.x = clipWidth - controlBounds.width - controlMargin.right;
            }
            else if ((control->_alignment & Control::ALIGN_HCENTER) == Control::ALIGN_HCENTER)
            {
                controlBounds.x = clipWidth * 0.5f - controlBounds.width * 0.5f;
            }
            else if ((control->_alignment & Control::ALIGN_LEFT) == Control::ALIGN_LEFT)
            {
                controlBounds.x = controlMargin.left;
            }
        }

        control->setBounds(controlBounds);
    }
}

bool Layout::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    return false;
}

}