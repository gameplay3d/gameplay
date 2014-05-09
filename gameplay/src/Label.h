#ifndef LABEL_H_
#define LABEL_H_

#include "Control.h"
#include "Theme.h"

namespace gameplay
{

/**
 * Defines a label control.
 * 
 * This is capable of rendering text within its border.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-UI_Forms
 */
class Label : public Control
{
    friend class Container;
	friend class ControlFactory;

public:

    /**
     * Creates a new label.
     *
     * @param id The label id.
     * @param style The label style (optional).
     *
     * @return The new label.
     * @script{create}
     */
    static Label* create(const char* id, Theme::Style* style = NULL);

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

    /**
     * @see Control::getType
     */
    const char* getType() const;

    /**
     * Add a listener to be notified of specific events affecting
     * this control.  Event types can be OR'ed together.
     * E.g. To listen to touch-press and touch-release events,
     * pass <code>Control::Listener::TOUCH | Control::Listener::RELEASE</code>
     * as the second parameter.
     *
     * @param listener The listener to add.
     * @param eventFlags The events to listen for.
     */
    virtual void addListener(Control::Listener* listener, int eventFlags);

protected:

    /**
     * Constructor.
     */
    Label();

    /**
     * Destructor.
     */
    virtual ~Label();

	/**
	* Create a new label control.
	*
	* @param style The control's custom style.
	* @param properties A properties object containing a definition of the label (optional).
	*
	* @return The new label.
	* @script{create}
	*/
	static Control* create(Theme::Style* style, Properties* properties);

    /**
     * @see Control::initialize
     */
    void initialize(const char* typeName, Theme::Style* style, Properties* properties);

    /**
     * @see Control::update
     */
    void update(float elapsedTime);

    /**
     * @see Control::updateState
     */
    void updateState(State state);

    /**
     * @see Control::updateBounds
     */
    void updateBounds();

    /**
     * @see Control::updateAbsoluteBounds
     */
    void updateAbsoluteBounds(const Vector2& offset);

    /**
     * @see Control::drawText
     */
    virtual unsigned int drawText(Form* form, const Rectangle& clip);

    /**
     * The text displayed by this label.
     */
    std::string _text;

    /**
     * The font being used to display the label.
     */
    Font* _font;
    
    /**
     * The text color being used to display the label.
     */
    Vector4 _textColor;

    /**
     * The position and size of this control's text area, before clipping.  Used for text alignment.
     */
    Rectangle _textBounds;

private:

    /**
     * Constructor.
     */
    Label(const Label& copy);
};

}

#endif
