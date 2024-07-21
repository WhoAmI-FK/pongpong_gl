#include "Sound.h"

#include <iostream>

SoundSrc::SoundSrc(std::string path)
{
    if(SDL_LoadWAV(path.c_str(), &m_audioSpec, &m_waveStart, &m_waveLength) == nullptr)
    {
        std::cerr << "Sound loading error: " << SDL_GetError() << std::endl;
    }
}

SoundSrc::~SoundSrc()
{
    SDL_FreeWAV(m_waveStart);
    SDL_CloseAudioDevice(m_device);
}

void SoundSrc::PlaySound()
{
    int status = SDL_QueueAudio(m_device, m_waveStart, m_waveLength);
    SDL_PauseAudioDevice(m_device, 0);
}

void SoundSrc::StopSound()
{
    SDL_PauseAudioDevice(m_device, 1);
}

void SoundSrc::SetupDevice()
{

    m_device = SDL_OpenAudioDevice(nullptr, 0, &m_audioSpec, nullptr, 0);

    if(0 == m_device)
    {
        std::cerr << "Sound device error: " << SDL_GetError() << std::endl;
    }
}
