#include "Base.h"
#include "AbsoluteLayout.h"

namespace gameplay
{
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
        AbsoluteLayout* al = new AbsoluteLayout();
        return al;
    }

    Layout::Type AbsoluteLayout::getType()
    {
        return Layout::LAYOUT_ABSOLUTE;
    }

    void AbsoluteLayout::update(std::vector<Control*> controls, const Vector2& size, Theme::Style* containerStyle)
    {
        // An AbsoluteLayout does nothing to modify the layout of Controls.
    }
}