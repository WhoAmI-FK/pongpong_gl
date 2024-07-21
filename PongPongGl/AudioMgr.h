#pragma once
#include <map>
#include <string>
#include <iostream>
#include <SDL.h>
#include "Sound.h"



class AudioMgr
{
private:
    std::map<std::string, ISound*> m_sounds;
    int m_currentTime;
    int m_lastTime;
public:
    void init();
    void update();
    void Play(std::string soundPath);
    ~AudioMgr();
    static std::string ballHit;
    static std::string wallCollide;
    static std::string menuSound;
};
