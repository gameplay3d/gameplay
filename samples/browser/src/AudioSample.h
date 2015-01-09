#ifndef FORMSSAMPLE_H_
#define FORMSSAMPLE_H_

#include "SamplesGame.h"

using namespace gameplay;

class AudioSample : public Sample, Control::Listener
{
public:
    
    AudioSample();

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

    void controlEvent(Control* control, EventType evt);

private:
    
    Form* _formBackground;
    Form* _formBraking;
    Form* _formEngine;
    AudioSource* _audioBackgroundSource;
    AudioSource* _audioBrakingSource;
    AudioSource* _audioEngineSource;
};

#endif
