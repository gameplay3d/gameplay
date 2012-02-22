#ifndef LAYOUT_H_
#define LAYOUT_H_

#include "Ref.h"

namespace gameplay
{

class Container;

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

    virtual void update(const Container* container) = 0;

protected:
    //Layout();
    //Layout(const Layout& copy);
    //virtual ~Layout();
};

}

#endif