#ifndef ABSOLUTELAYOUT_H_
#define ABSOLUTELAYOUT_H_

#include "Layout.h"

namespace gameplay
{

class AbsoluteLayout : public Layout
{
public:
    static AbsoluteLayout* create();

    Layout::Type getType();

    void update(std::vector<Control*> controls, const Vector2& size, Theme::Style* containerStyle);

private:
    AbsoluteLayout();
    AbsoluteLayout(const AbsoluteLayout& copy);
    virtual ~AbsoluteLayout();
};

}

#endif