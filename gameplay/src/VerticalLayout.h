#ifndef VERTICALLAYOUT_H_
#define VERTICALLAYOUT_H_

#include "Layout.h"

namespace gameplay
{

class VerticalLayout : public Layout
{
public:
    static VerticalLayout* create();

    void setBottomToTop(bool bottomToTop);

    Layout::Type getType();

    void update(std::vector<Control*> controls, const Vector2& size, Theme::Style* containerStyle);

private:
    VerticalLayout();
    VerticalLayout(const VerticalLayout& copy);
    virtual ~VerticalLayout();

    bool _bottomToTop;
};

}

#endif