#include "Base.h"
#include "Audio.h"
#include "AudioOpenAL.h"

namespace gameplay
{

Audio* Audio::_audio = nullptr;

Audio* Audio::getAudio()
{
    if (!_audio)
    {
        _audio = new AudioOpenAL();
    }
    return _audio;
}

}
