#ifndef LAYOUT_H_
#define LAYOUT_H_

#include "Ref.h"
#include "Control.h"

namespace gameplay
{

class Layout : public Ref
{
public:
    enum Type
    {
        LAYOUT_FLOW,
        LAYOUT_VERTICAL,
        LAYOUT_ABSOLUTE
    };

    virtual Type getType() = 0;

    virtual void update(std::vector<Control*> controls, const Vector2& size, Theme::Style* containerStyle) = 0;

protected:
    //Layout();
    //Layout(const Layout& copy);
    //virtual ~Layout();
};

}

#endif