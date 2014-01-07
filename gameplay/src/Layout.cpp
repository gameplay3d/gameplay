#include "Base.h"
#include "Layout.h"
#include "Control.h"
#include "Container.h"
#include "Game.h"

namespace gameplay
{

void Layout::align(Control* control, const Container* container)
{
    GP_ASSERT(control);

    if (control->_alignment != Control::ALIGN_TOP_LEFT ||
        control->_isAlignmentSet)
    {
        const Rectangle& controlBounds = control->getBounds();
        const Theme::Margin& controlMargin = control->getMargin();
        const Rectangle& containerBounds = container ? container->getBounds() : Game::getInstance()->getViewport();
        const Theme::Border& containerBorder = container ? container->getBorder(container->getState()) : Theme::Border::empty();
        const Theme::Padding& containerPadding = container ? container->getPadding() : Theme::Padding::empty();

        float clipWidth;
        float clipHeight; 
        if (container && (container->getScroll() != Container::SCROLL_NONE))
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

        // Vertical alignment
        if (control->_isAlignmentSet || control->_autoHeight)
        {
            if ((control->_alignment & Control::ALIGN_BOTTOM) == Control::ALIGN_BOTTOM)
            {
                control->setY(clipHeight - controlBounds.height - controlMargin.bottom);
            }
            else if ((control->_alignment & Control::ALIGN_VCENTER) == Control::ALIGN_VCENTER)
            {
                control->setY(clipHeight * 0.5f - controlBounds.height * 0.5f);
            }
            else if ((control->_alignment & Control::ALIGN_TOP) == Control::ALIGN_TOP)
            {
                control->setY(controlMargin.top);
            }
        }

        // Horizontal alignment
        if (control->_isAlignmentSet)
        {
            if ((control->_alignment & Control::ALIGN_RIGHT) == Control::ALIGN_RIGHT)
            {
                control->setX(clipWidth - controlBounds.width - controlMargin.right);
            }
            else if ((control->_alignment & Control::ALIGN_HCENTER) == Control::ALIGN_HCENTER)
            {
                control->setX(clipWidth * 0.5f - controlBounds.width * 0.5f);
            }
            else if ((control->_alignment & Control::ALIGN_LEFT) == Control::ALIGN_LEFT)
            {
                control->setX(controlMargin.left);
            }
        }
    }
}

bool Layout::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    return false;
}

}