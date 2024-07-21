#pragma once

#include <string>
#include <SDL.h>

class ISound {
public:
    virtual ~ISound() {};
    virtual void PlaySound() = 0;
    virtual void StopSound() = 0;
};

class SoundSrc : public ISound{
public:
    SoundSrc(std::string path);
    ~SoundSrc();

    void PlaySound() override;
    void StopSound() override;
    void SetupDevice();

private:
    SDL_AudioDeviceID m_device;
    SDL_AudioSpec m_audioSpec;
    Uint8* m_waveStart;
    Uint32 m_waveLength;
};
