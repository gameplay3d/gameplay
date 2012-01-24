#include "Base.h"
#include "VerticalLayout.h"

namespace gameplay
{
    VerticalLayout::VerticalLayout()
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
        VerticalLayout* al = new VerticalLayout();
        return al;
    }

    void VerticalLayout::setBottomToTop(bool bottomToTop)
    {
        _bottomToTop = bottomToTop;
    }

    Layout::Type VerticalLayout::getType()
    {
        return Layout::LAYOUT_VERTICAL;
    }

    void VerticalLayout::update(std::vector<Control*> controls, const Vector2& size)
    {
        float yPosition = 0.0f;

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
            const Vector2& size = control->getSize();
            const Theme::Margin& margin = control->getStyle()->getMargin();

            yPosition += margin.top;

            control->setPosition(0.0f, yPosition);

            yPosition += size.y + margin.bottom;

            i += iter;
        }
    }
}