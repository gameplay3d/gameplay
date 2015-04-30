#ifndef AUDIOSAMPLE_H_
#define AUDIOSAMPLE_H_

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
    Form* _formEngine;
    Form* _formBraking;
    AudioSource* _audioBackground;
    AudioSource* _audioEngine;
    AudioSource* _audioBraking;
};

#endif
