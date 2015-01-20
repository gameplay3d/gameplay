#include "AudioSample.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Media", "Audio Player", AudioSample, 2);
#endif

AudioSample::AudioSample()
    :  _formBackground(NULL), _formEngine(NULL), _formBraking(NULL),
       _audioBackground(NULL), _audioEngine(NULL), _audioBraking(NULL)
{
}

void AudioSample::initialize()
{
    _formBackground = Form::create("res/common/audio/background.form");

    const char * buttons[] = { "playButton", "pauseButton", "resumeButton", "rewindButton", "stopButton" };
    for (int i = 0; i < sizeof(buttons) / sizeof(uintptr_t); i++) {
        Button* button = static_cast<Button*>(_formBackground->getControl(buttons[i]));
        button->addListener(this, Control::Listener::RELEASE);
    }   

    // Create the audio source here, and feed the values into the UI controls.
    _audioBackground = AudioSource::create("res/common/audio/sample.audio#backgroundTrack");
    _audioBackground->play();

    CheckBox* checkBox = static_cast<CheckBox*>(_formBackground->getControl("loopCheckBox"));
    checkBox->setChecked(_audioBackground->isLooped());
    checkBox->addListener(this, Control::Listener::VALUE_CHANGED);

    Slider* slider = static_cast<Slider*>(_formBackground->getControl("gainSlider"));
    _audioBackground->setGain(slider->getValue());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_formBackground->getControl("pitchSlider"));
    slider->setValue(_audioBackground->getPitch());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    _formBraking = Form::create("res/common/audio/braking.form");

    Button* button = static_cast<Button*>(_formBraking->getControl("playBrakingButton"));
    button->addListener(this, Control::Listener::RELEASE);

    button = static_cast<Button*>(_formBraking->getControl("stopBrakingButton"));
    button->addListener(this, Control::Listener::RELEASE);

    _formEngine = Form::create("res/common/audio/engine.form");

    button = static_cast<Button*>(_formEngine->getControl("playEngineButton"));
    button->addListener(this, Control::Listener::RELEASE);

    button = static_cast<Button*>(_formEngine->getControl("stopEngineButton"));
    button->addListener(this, Control::Listener::RELEASE);

    _audioEngine = AudioSource::create("res/common/audio/sample.audio#engine");

    checkBox = static_cast<CheckBox*>(_formEngine->getControl("loopEngineCheckBox"));
    checkBox->setChecked(_audioEngine->isLooped());
    checkBox->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_formEngine->getControl("gainEngineSlider"));
    slider->setValue(_audioEngine->getGain());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_formEngine->getControl("pitchEngineSlider"));
    slider->setValue(_audioEngine->getPitch());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);
    
    _audioBraking = AudioSource::create("res/common/audio/sample.audio#braking");
    
    checkBox = static_cast<CheckBox*>(_formBraking->getControl("loopBrakingCheckBox"));
    checkBox->setChecked(_audioBraking->isLooped());
    checkBox->addListener(this, Control::Listener::VALUE_CHANGED);
    
    slider = static_cast<Slider*>(_formBraking->getControl("gainBrakingSlider"));
    slider->setValue(_audioBraking->getGain());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);
    
    slider = static_cast<Slider*>(_formBraking->getControl("pitchBrakingSlider"));
    slider->setValue(_audioBraking->getPitch());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);
}

void AudioSample::finalize()
{
    SAFE_RELEASE(_audioBraking);
    SAFE_RELEASE(_audioEngine);
    SAFE_RELEASE(_audioBackground);
    
    SAFE_RELEASE(_formBraking);
    SAFE_RELEASE(_formEngine);
    SAFE_RELEASE(_formBackground);
}

void AudioSample::update(float elapsedTime)
{
    if (_formBackground)
        _formBackground->update(elapsedTime);
    if (_formEngine)
        _formEngine->update(elapsedTime);
    if (_formBraking)
        _formBraking->update(elapsedTime);
}

void AudioSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Draw all the forms
    _formBackground->draw();
    _formEngine->draw();
    _formBraking->draw();
}

void AudioSample::controlEvent(Control* control, EventType eventType)
{
    switch (eventType)
    {
    case Control::Listener::RELEASE:
        if (strcmp("playButton", control->getId()) == 0)
        {
            _audioBackground->play();
        }
        else if (strcmp("pauseButton", control->getId()) == 0)
        {
            _audioBackground->pause();
        }
        else if (strcmp("resumeButton", control->getId()) == 0)
        {
            _audioBackground->resume();
        }
        else if (strcmp("rewindButton", control->getId()) == 0)
        {
            _audioBackground->rewind();
        }
        else if (strcmp("stopButton", control->getId()) == 0)
        {
            _audioBackground->stop();
        }
        else if (strcmp("playEngineButton", control->getId()) == 0)
        {
            _audioEngine->play();
        }
        else if (strcmp("stopEngineButton", control->getId()) == 0)
        {
            _audioEngine->stop();
        }
        else if (strcmp("playBrakingButton", control->getId()) == 0)
        {
            _audioBraking->play();
        }
        else if (strcmp("stopBrakingButton", control->getId()) == 0)
        {
            _audioBraking->stop();
        }
        break;
    case Control::Listener::VALUE_CHANGED:
        if (strcmp("loopCheckBox", control->getId()) == 0)
        {
            CheckBox* loopCheckBox = static_cast<CheckBox*>(control);
            _audioBackground->setLooped(loopCheckBox->isChecked());
        }
        else if (strcmp("gainSlider", control->getId()) == 0)
        {
            Slider* gainSlider = static_cast<Slider*>(control);
            _audioBackground->setGain(float(gainSlider->getValue()));
        }
        else if (strcmp("pitchSlider", control->getId()) == 0)
        {
            Slider* pitchSlider = static_cast<Slider*>(control);
            float pitchValue = (float)pitchSlider->getValue();
            if (pitchValue != 0.0f)
            {
                _audioBackground->setPitch(pitchValue);
            }
        }
        else if (strcmp("loopEngineCheckBox", control->getId()) == 0)
        {
            CheckBox* loopCheckBox = static_cast<CheckBox*>(control);
            _audioEngine->setLooped(loopCheckBox->isChecked());
        }
        else if (strcmp("gainEngineSlider", control->getId()) == 0)
        {
            Slider* gainSlider = static_cast<Slider*>(control);
            _audioEngine->setGain(float(gainSlider->getValue()));
        }
        else if (strcmp("pitchEngineSlider", control->getId()) == 0)
        {
            Slider* pitchSlider = static_cast<Slider*>(control);
            float pitchValue = (float)pitchSlider->getValue();
            if (pitchValue != 0.0f)
            {
                _audioEngine->setPitch(pitchValue);
            }
        }
        else if (strcmp("loopBrakingCheckBox", control->getId()) == 0)
        {
            CheckBox* loopCheckBox = static_cast<CheckBox*>(control);
            _audioBraking->setLooped(loopCheckBox->isChecked());
        }
        else if (strcmp("gainBrakingSlider", control->getId()) == 0)
        {
            Slider* gainSlider = static_cast<Slider*>(control);
            _audioBraking->setGain(float(gainSlider->getValue()));
        }
        else if (strcmp("pitchBrakingSlider", control->getId()) == 0)
        {
            Slider* pitchSlider = static_cast<Slider*>(control);
            float pitchValue = (float)pitchSlider->getValue();
            if (pitchValue != 0.0f)
            {
                _audioBraking->setPitch(pitchValue);
            }
        }
        break;
    }
}
