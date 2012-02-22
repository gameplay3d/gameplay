#ifndef FLOWLAYOUT_H_
#define FLOWLAYOUT_H_

#include "Layout.h"

namespace gameplay
{

class FlowLayout : public Layout
{
public:
    static FlowLayout* create();

    void setRightToLeft(bool rightToLeft);

private:
    FlowLayout();
    FlowLayout(const FlowLayout& copy);
    virtual ~FlowLayout();
};

}

#endif