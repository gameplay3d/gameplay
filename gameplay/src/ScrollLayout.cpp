#include "Base.h"
#include "Control.h"
#include "ScrollLayout.h"
#include "Container.h"

namespace gameplay
{

ScrollLayout::ScrollLayout() 
    : _scrollPosition(Vector2::zero()), _lastX(0), _lastY(0), _scrolling(false),
      _positionVertically(false), _positionHorizontally(false)
{
}

ScrollLayout::ScrollLayout(const ScrollLayout& copy)
{
}

ScrollLayout::~ScrollLayout()
{
}

ScrollLayout* ScrollLayout::create()
{
    return new ScrollLayout();
}

Layout::Type ScrollLayout::getType()
{
    return Layout::LAYOUT_SCROLL;
}

void ScrollLayout::update(const Container* container)
{
    // Position controls if automatic positioning is enabled.
    if (_positionVertically && _positionHorizontally)
    {
        // Treat as scrollable flow layout.
    }
    else if (_positionVertically)
    {
        // Scrollable vertical layout.
    }
    else if (_positionHorizontally)
    {
        // Scrollable horizontal layout.
    }

    // Calculate total width and height.
    float totalWidth = 0;
    float totalHeight = 0;
    std::vector<Control*> controls = container->getControls();
    unsigned int controlsCount = controls.size();
    for (unsigned int i = 0; i < controlsCount; i++)
    {
        Control* control = controls.at(i);

        const Rectangle& bounds = control->getBounds();
        const Theme::Margin& margin = control->getMargin();

        float newWidth = bounds.x + bounds.width + margin.left + margin.right;
        if (newWidth > totalWidth)
        {
            totalWidth = newWidth;
        }

        float newHeight = bounds.y + bounds.height + margin.top + margin.bottom;
        if (newHeight > totalHeight)
        {
            totalHeight = newHeight;
        }
    }

    const Rectangle& containerBounds = container->getBounds();
    const Theme::Border& containerBorder = container->getBorder(container->getState());
    const Theme::Padding& containerPadding = container->getPadding();

    float clipWidth = containerBounds.width - containerBorder.left - containerBorder.right - containerPadding.left - containerPadding.right;
    float clipHeight = containerBounds.height - containerBorder.top - containerBorder.bottom - containerPadding.top - containerPadding.bottom;

    // Stop scrolling when the far edge is reached.
    if (-_scrollPosition.x > totalWidth - clipWidth)
    {
        _scrollPosition.x = -(totalWidth - clipWidth);
    }
    
    if (-_scrollPosition.y > totalHeight - clipHeight)
    {
        _scrollPosition.y = -(totalHeight - clipHeight);
    }

    if (_scrollPosition.x > 0)
    {
        _scrollPosition.x = 0;
    }

    if (_scrollPosition.y > 0)
    {
        _scrollPosition.y = 0;
    }

    // Position controls within scroll area.
    for (unsigned int i = 0; i < controlsCount; i++)
    {
        Control* control = controls.at(i);
        control->update(container->getClip(), _scrollPosition);
    }
}

bool ScrollLayout::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch(evt)
    {
    case Touch::TOUCH_PRESS:
        _lastX = x;
        _lastY = y;
        _scrolling = true;
        break;

    case Touch::TOUCH_MOVE:
        if (_scrolling)
        {
            // Calculate the latest movement delta for the next update to use.
            _scrollPosition.x += x - _lastX;
            _scrollPosition.y += y - _lastY;
            _lastX = x;
            _lastY = y;
            return true;
        }
        break;

    case Touch::TOUCH_RELEASE:
        _scrolling = false;
        break;
    }

    return false;
}

}