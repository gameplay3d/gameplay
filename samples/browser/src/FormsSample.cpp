#include "FormsSample.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Forms", FormsSample, 8);
#endif

// Input bit-flags
#define KEY_BACK 1

const static unsigned int __formsCount = 5;

FormsSample::FormsSample()
    : _scene(NULL), _formNode(NULL), _formNodeParent(NULL), _formSelect(NULL), _activeForm(NULL), _gamepad(NULL), _keyFlags(0)
{
    const char* formFiles[] = 
    {
        "res/common/forms/formBasicControls.form",
        "res/common/forms/formScrolling.form",
        "res/common/forms/formFlowLayout.form",
        "res/common/forms/formVerticalLayout.form",
        "res/common/forms/formZOrder.form",
    };

    _formFiles.assign(formFiles, formFiles + __formsCount);
}

void FormsSample::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_formNode);
    SAFE_RELEASE(_formSelect);
    for (unsigned int i = 0; i < _forms.size(); i++)
    {
        SAFE_RELEASE(_forms[i]);
    }
}

void printProperties(Properties* properties, unsigned int tabCount)
{
    // Print the name and ID of the current namespace.
    const char* spacename = properties->getNamespace();
    const char* id = properties->getId();
    std::string tabs;
    for (unsigned int i = 0; i < tabCount; i++)
    {
        tabs.append("\t");
    }
    GP_WARN("\n%s%s %s\n%s{", tabs.c_str(), spacename, id, tabs.c_str());
 
    // Print all properties in this namespace.
    const char* name = properties->getNextProperty();
    const char* value = NULL;
    while (name != NULL)
    {
        value = properties->getString(name);
        GP_WARN("%s\t%s = %s", tabs.c_str(), name, value);
        name = properties->getNextProperty();
    }
 
    // Print the properties of every namespace within this one.
    Properties* space = properties->getNextNamespace();
    while (space != NULL)
    {
        printProperties(space, tabCount+1);
        space = properties->getNextNamespace();
    }

    GP_WARN("%s}", tabs.c_str());
}

void FormsSample::initialize()
{
    setMultiTouch(true);
    setVsync(false);

    _formSelect = Form::create("res/common/forms/formSelect.form");
    _formSelect->setFocus();

    RadioButton* form0Button = static_cast<RadioButton*>(_formSelect->getControl("form0"));
    form0Button->addListener(this, Control::Listener::CLICK);

    RadioButton* form1Button = static_cast<RadioButton*>(_formSelect->getControl("form1"));
    form1Button->addListener(this, Control::Listener::CLICK);

    RadioButton* form2Button = static_cast<RadioButton*>(_formSelect->getControl("form2"));
    form2Button->addListener(this, Control::Listener::CLICK);
    
    RadioButton* form3Button = static_cast<RadioButton*>(_formSelect->getControl("form3"));
    form3Button->addListener(this, Control::Listener::CLICK);

    RadioButton* form4Button = static_cast<RadioButton*>(_formSelect->getControl("form4"));
    form4Button->addListener(this, Control::Listener::CLICK);
    
    RadioButton* form5Button = static_cast<RadioButton*>(_formSelect->getControl("form5"));
    form5Button->addListener(this, Control::Listener::CLICK);
    for (unsigned int i = 0; i < _formFiles.size(); i++)
    {
		Form* form = Form::create(_formFiles[i]);
        form->setEnabled(false);
        _forms.push_back(form);
    }
    _formIndex = 0;

    // Create a form programmatically.
    createSampleForm();

    Button* button = static_cast<Button*>(_forms[0]->getControl("testButton"));
    button->setFocus();

    // Create a scene with a camera node.
    Camera* camera = Camera::createPerspective(45.0f, (float)getWidth() / (float)getHeight(), 0.25f, 100.0f);
    _scene = Scene::create();
    Node* cameraNode = _scene->addNode("Camera");
    cameraNode->setCamera(camera);
    _scene->setActiveCamera(camera);
    SAFE_RELEASE(camera);
    _formNodeParent = _scene->addNode("FormParent");
    _formNode = Node::create("Form");
    _formNodeParent->addChild(_formNode);
    
    formChanged();

    _gamepad = getGamepad(0);
    // This is needed because the virtual gamepad is shared between all samples.
    // SamplesGame always ensures the virtual gamepad is disabled when a sample is exited.
    if (_gamepad && _gamepad->isVirtual())
        _gamepad->getForm()->setEnabled(true);
}

void FormsSample::formChanged()
{
    if (_activeForm)
        _activeForm->setEnabled(false);
    _activeForm = _forms[_formIndex];
    _activeForm->setEnabled(true);
    _activeForm->setFocus();

    // Add the form to a node to allow it to be placed in 3D space.
    const Rectangle& bounds = _activeForm->getBounds();
    float scale;
    if (bounds.width >= bounds.height)
    {
        scale = 1.0f / bounds.width;
    }
    else
    {
        scale = 1.0f / bounds.height;
    }

    _formNode->setScale(scale, scale, 1.0f);
    _formNodeParent->setTranslation(0, 0, -1.5f);
    _formNode->setTranslation(-0.5f, -0.5f, 0);
    _formNode->setDrawable(_activeForm);
}

void FormsSample::createSampleForm()
{
    Form* form = Form::create("testForm", NULL);
    form->setSize(600, 600);

    Label* label = Label::create("testLabel");
    label->setPosition(50, 50);
    label->setSize(200, 50);
    label->setText("Label:");
    form->addControl(label);
    label->release();

    Button* button = Button::create("opacityButton");
    button->setPosition(45, 100);
    button->setSize(200, 100);
    button->setText("This is a button.  Click to change its opacity.");
    button->addListener(this, Control::Listener::CLICK);
    form->addControl(button);
    button->release();

    form->setEnabled(false);
    _forms.push_back(form);
}

void FormsSample::update(float elapsedTime)
{
    float speedFactor = 0.001f * elapsedTime;

    _gamepad->getJoystickValues(0, &_joysticks[0]);
    _gamepad->getJoystickValues(1, &_joysticks[1]);

    // We'll use a physical gamepad's MENU1 button as the "back" button.
    // Toggle focus between the active form, the selection form and no focus.
    if (_gamepad->isButtonDown(Gamepad::BUTTON_MENU1))
    {
        if (!(_keyFlags & KEY_BACK))
        {
            _keyFlags |= KEY_BACK;
            if (Form::getFocusControl())
            {
                if (Form::getFocusControl() == _activeForm || Form::getFocusControl()->isChild(_activeForm))
                    _formSelect->setFocus();
                else
                    Form::clearFocus();
            }
            else
            {
                if (!(_activeForm && _activeForm->setFocus()))
                    _formSelect->setFocus();
            }
        }
    }
    else
    {
        _keyFlags &= ~KEY_BACK;
    }

    // If no controls are in focus, then we poll the gamepad for movement input.
    if (Form::getFocusControl() == NULL)
    {
        if (!_joysticks[0].isZero())
        {
            _formNodeParent->translate(0.5f * speedFactor * _joysticks[0].x, 0.5f * speedFactor * _joysticks[0].y, 0);
        }

        if (!_joysticks[1].isZero())
        {
            Matrix m;
            _formNodeParent->getWorldMatrix().transpose(&m);
            Vector3 yaw;
            m.getUpVector(&yaw);
            _formNodeParent->rotate(yaw, speedFactor * _joysticks[1].x * 2.0f);
            _formNodeParent->rotateX(-speedFactor * _joysticks[1].y * 2.0f);
        }
    }

    if (_gamepad->isButtonDown(Gamepad::BUTTON_A))
    {
        _formNodeParent->setTranslation(0, 0, -1.5f);
    }
    if (_gamepad->isButtonDown(Gamepad::BUTTON_B))
    {
        _formNodeParent->setRotation(0, 0, 0, 1);
    }
}

void FormsSample::render(float elapsedTime)
{
    // Clear the screen.
    clear(CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 1), 1.0f, 0);

    // Draw the forms.
    if (_formSelect)
    {
        _formSelect->draw();
    }

    if (_activeForm)
    {
        _activeForm->draw();
    }

    _gamepad->draw();
}

void FormsSample::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (_formNode)
    {
        switch (evt)
        {
        case Touch::TOUCH_PRESS:
            {
                _touched = true;
                _touchX = x;
            }
            break;
        case Touch::TOUCH_RELEASE:
            {
                _touched = false;
                _touchX = 0;
            }
            break;
        case Touch::TOUCH_MOVE:
            {
                int deltaX = x - _touchX;
                _touchX = x;
                // Yaw in world frame
                Matrix m;
                _formNodeParent->getWorldMatrix().transpose(&m);
                Vector3 yaw;
                m.getUpVector(&yaw);
                _formNodeParent->rotate(yaw, MATH_DEG_TO_RAD(deltaX * 0.5f));
            }
            break;
        default:
            break;
        };
    }
}

void FormsSample::controlEvent(Control* control, EventType evt)
{
    if (evt == CLICK)
    {
        if (strcmp("form0", control->getId()) == 0)
        {
            _formIndex = 0;
            formChanged();
        }
        else if (strcmp("form1", control->getId()) == 0)
        {
            _formIndex = 1;
            formChanged();
        }
        else if (strcmp("form2", control->getId()) == 0)
        {
            _formIndex = 2;
            formChanged();
        }
        else if (strcmp("form3", control->getId()) == 0)
        {
            _formIndex = 3;
            formChanged();
        }
        else if (strcmp("form4", control->getId()) == 0)
        {
            _formIndex = 4;
            formChanged();
        }
        else if (strcmp("form5", control->getId()) == 0)
        {
            _formIndex = 5;
            formChanged();
        }
        else if (strcmp("opacityButton", control->getId()) == 0)
        {
            float from[] = { 1.0f };
            float to[] = { 0.5f };
            control->createAnimationFromTo("opacityButton", Form::ANIMATE_OPACITY, from, to, Curve::LINEAR, 1000)->getClip()->play();
        }
    }
}

void FormsSample::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
    switch(evt)
    {
    case Gamepad::CONNECTED_EVENT:
    case Gamepad::DISCONNECTED_EVENT:
        _gamepad = getGamepad(0);
        // This is needed because the virtual gamepad is shared between all samples.
        // SamplesGame always ensures the virtual gamepad is disabled when a sample is exited.
        if (_gamepad && _gamepad->isVirtual())
            _gamepad->getForm()->setEnabled(true);
        break;
    }
}
