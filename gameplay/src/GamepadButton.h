#ifndef GAMEPADBUTTON_H_
#define GAMEPADBUTTON_H_

#include "Button.h"
#include "Theme.h"
#include "Properties.h"
#include "Gamepad.h"

namespace gameplay
{

class GamepadButton : public Button
{
    friend class Container;
    friend class Gamepad;

public:
    static GamepadButton* create(const char* id, Theme::Style* style);

protected:
    /**
     * Constructor.
     */
    GamepadButton();

    /**
     * Destructor.
     */
    virtual ~GamepadButton();

    static GamepadButton* create(Theme::Style* style, Properties* properties);

    const char* getType() const;

    const Gamepad::ButtonMapping getMapping() const;

private:
    Gamepad::ButtonMapping _mapping;
};

}

#endif