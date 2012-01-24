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

    void update(std::vector<Control*> controls, const Vector2& size);

private:
    AbsoluteLayout();
    AbsoluteLayout(const AbsoluteLayout& copy);
    virtual ~AbsoluteLayout();
};

}

#endif