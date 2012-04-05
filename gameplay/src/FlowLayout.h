#ifndef FLOWLAYOUT_H_
#define FLOWLAYOUT_H_

#include "Layout.h"

namespace gameplay
{

/**
 *  Defines a Layout for forms and containers that arranges controls 
 * in a left-to-right(default) or right-to-left flow.
 */
class FlowLayout : public Layout
{
public:

    /**
     * Constructor.
     */
    static FlowLayout* create();

    /**
     * Sets if the layout is left-to-right or right to left.
     */
    void setRightToLeft(bool rightToLeft);

private:
    
    /**
     * Constructor.
     */
    FlowLayout();
    
    /**
     * Constructor.
     */
    FlowLayout(const FlowLayout& copy);
    
    /**
     * Destructor.
     */
    virtual ~FlowLayout();
};

}

#endif