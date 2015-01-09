#include "AudioSample.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Media", "Audio Player", AudioSample, 2);
#endif

AudioSample::AudioSample()
    :  _formBackground(NULL), _formBraking(NULL),  _formEngine(NULL), _audioBackgroundSource(NULL), _audioBrakingSource(NULL), _audioEngineSource(NULL)
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
    _audioBackgroundSource = AudioSource::create("res/common/audio/sample.audio#backgroundTrack");
    _audioBackgroundSource->play();

    CheckBox* checkBox = static_cast<CheckBox*>(_formBackground->getControl("loopCheckBox"));
    checkBox->setChecked(_audioBackgroundSource->isLooped());
    checkBox->addListener(this, Control::Listener::VALUE_CHANGED);

    Slider* slider = static_cast<Slider*>(_formBackground->getControl("gainSlider"));
    slider->setValue(_audioBackgroundSource->getGain());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_formBackground->getControl("pitchSlider"));
    slider->setValue(_audioBackgroundSource->getPitch());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    _formBraking = Form::create("res/common/audio/braking.form");

    Button* button = static_cast<Button*>(_formBraking->getControl("playBrakingButton"));
    button->addListener(this, Control::Listener::RELEASE);

    button = static_cast<Button*>(_formBraking->getControl("stopBrakingButton"));
    button->addListener(this, Control::Listener::RELEASE);

    _audioBrakingSource = AudioSource::create("res/common/audio/sample.audio#braking");

    checkBox = static_cast<CheckBox*>(_formBraking->getControl("loopBrakingCheckBox"));
    checkBox->setChecked(_audioBrakingSource->isLooped());
    checkBox->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_formBraking->getControl("gainBrakingSlider"));
    slider->setValue(_audioBrakingSource->getGain());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_formBraking->getControl("pitchBrakingSlider"));
    slider->setValue(_audioBrakingSource->getPitch());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    _formEngine = Form::create("res/common/audio/engine.form");

    button = static_cast<Button*>(_formEngine->getControl("playEngineButton"));
    button->addListener(this, Control::Listener::RELEASE);

    button = static_cast<Button*>(_formEngine->getControl("stopEngineButton"));
    button->addListener(this, Control::Listener::RELEASE);

    _audioEngineSource = AudioSource::create("res/common/audio/sample.audio#engine");

    checkBox = static_cast<CheckBox*>(_formEngine->getControl("loopEngineCheckBox"));
    checkBox->setChecked(_audioEngineSource->isLooped());
    checkBox->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_formEngine->getControl("gainEngineSlider"));
    slider->setValue(_audioEngineSource->getGain());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);

    slider = static_cast<Slider*>(_formEngine->getControl("pitchEngineSlider"));
    slider->setValue(_audioEngineSource->getPitch());
    slider->addListener(this, Control::Listener::VALUE_CHANGED);
}

void AudioSample::finalize()
{
    SAFE_RELEASE(_audioEngineSource);
    SAFE_RELEASE(_formEngine);
    SAFE_RELEASE(_audioBackgroundSource);
    SAFE_RELEASE(_formBraking);
    SAFE_RELEASE(_audioBackgroundSource);
    SAFE_RELEASE(_formBackground);
}

void AudioSample::update(float elapsedTime)
{
    _formBackground->update(elapsedTime);
    _formBraking->update(elapsedTime);
    _formEngine->update(elapsedTime);
}

void AudioSample::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _formBackground->draw();
    _formBraking->draw();
    _formEngine->draw();
}

void AudioSample::controlEvent(Control* control, EventType eventType)
{
    switch (eventType)
    {
    case Control::Listener::RELEASE:
        if (strcmp("playButton", control->getId()) == 0) {
            _audioBackgroundSource->play();
        } else if (strcmp("pauseButton", control->getId()) == 0) {
            _audioBackgroundSource->pause();
        } else if (strcmp("resumeButton", control->getId()) == 0) {
            _audioBackgroundSource->resume();
        } else if (strcmp("rewindButton", control->getId()) == 0) {
            _audioBackgroundSource->rewind();
        } else if (strcmp("stopButton", control->getId()) == 0) {
            _audioBackgroundSource->stop();
        } else if (strcmp("playBrakingButton", control->getId()) == 0) {
            _audioBrakingSource->play();
        } else if (strcmp("stopBrakingButton", control->getId()) == 0) {
            _audioBrakingSource->stop();
        } else if (strcmp("playEngineButton", control->getId()) == 0) {
            _audioEngineSource->play();
        } else if (strcmp("stopEngineButton", control->getId()) == 0) {
            _audioEngineSource->stop();
        }
        break;
    case Control::Listener::VALUE_CHANGED:
        if (strcmp("loopCheckBox", control->getId()) == 0) {
            CheckBox* loopCheckBox = static_cast<CheckBox*>(control);
            _audioBackgroundSource->setLooped(loopCheckBox->isChecked());
        } else if (strcmp("gainSlider", control->getId()) == 0) {
            Slider* gainSlider = static_cast<Slider*>(control);
            _audioBackgroundSource->setGain(float(gainSlider->getValue()));
        } else if (strcmp("pitchSlider", control->getId()) == 0) {
            Slider* pitchSlider = static_cast<Slider*>(control);
            float pitchValue = (float)pitchSlider->getValue();
            if (pitchValue != 0.0f) {
                _audioBackgroundSource->setPitch(pitchValue);
            }
        } else if (strcmp("loopBrakingCheckBox", control->getId()) == 0) {
            CheckBox* loopCheckBox = static_cast<CheckBox*>(control);
            _audioBrakingSource->setLooped(loopCheckBox->isChecked());
        } else if (strcmp("gainBrakingSlider", control->getId()) == 0) {
            Slider* gainSlider = static_cast<Slider*>(control);
            _audioBrakingSource->setGain(float(gainSlider->getValue()));
        } else if (strcmp("pitchBrakingSlider", control->getId()) == 0) {
            Slider* pitchSlider = static_cast<Slider*>(control);
            float pitchValue = (float)pitchSlider->getValue();
            if (pitchValue != 0.0f) {
                _audioBrakingSource->setPitch(pitchValue);
            }
        } else if (strcmp("loopEngineCheckBox", control->getId()) == 0) {
            CheckBox* loopCheckBox = static_cast<CheckBox*>(control);
            _audioEngineSource->setLooped(loopCheckBox->isChecked());
        } else if (strcmp("gainEngineSlider", control->getId()) == 0) {
            Slider* gainSlider = static_cast<Slider*>(control);
            _audioEngineSource->setGain(float(gainSlider->getValue()));
        } else if (strcmp("pitchEngineSlider", control->getId()) == 0) {
            Slider* pitchSlider = static_cast<Slider*>(control);
            float pitchValue = (float)pitchSlider->getValue();
            if (pitchValue != 0.0f) {
                _audioEngineSource->setPitch(pitchValue);
            }
        }
        break;
    }
}
