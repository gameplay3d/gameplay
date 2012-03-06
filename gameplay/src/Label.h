#ifndef LABEL_H_
#define LABEL_H_

#include "Control.h"
#include "Theme.h"

namespace gameplay
{

/**
 * A label is the most basic type of control, capable only of rendering text within its border.
 *
 * The following properties are available for labels:
 *
 * label <Label ID>
 * {
 *      style       = <Style ID>
 *      position    = <x, y>
 *      size        = <width, height>
 *      text        = <string>
 * }
 */
class Label : public Control
{
    friend class Container;

public:
    /**
     * Set the text for this label to display.
     *
     * @param text The text to display.
     */
    void setText(const char* text);

    /**
     * Get the text displayed by this label.
     *
     * @return The text displayed by this label.
     */
    const char* getText();

protected:
    Label();
    virtual ~Label();

    /**
     * Create a label with a given style and properties.
     *
     * @param style The style to apply to this label.
     * @param properties The properties to set on this label.
     *
     * @return The new label.
     */
    static Label* create(Theme::Style* style, Properties* properties);

    /**
     * Initialize this label.
     */
    virtual void init(Theme::Style* style, Properties* properties);

    /**
     * Draw this label's text.
     *
     * @param clip The clipping rectangle of this label's parent container.
     */
    void drawText(const Rectangle& clip);

    std::string _text;  // The text displayed by this label.

private:
    Label(const Label& copy);
};

}

#endif