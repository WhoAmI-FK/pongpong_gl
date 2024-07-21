#include "AudioMgr.h"

//SoundSrc* sCollide;
SoundSrc* ballHitSnd;
SoundSrc* wallCollideSnd;
SoundSrc* menuSoundSnd;
std::string AudioMgr::ballHit = "sounds\\ballHit.wav";
std::string AudioMgr::wallCollide = "sounds\\CollideOrg.wav";
std::string AudioMgr::menuSound = "sounds\\MenuButton.wav";

void AudioMgr::init()
{
    m_currentTime = SDL_GetTicks();
    m_lastTime = SDL_GetTicks();

//    sCollide = new SoundSrc("Collide.wav");
//    sCollide->SetupDevice();
    ballHitSnd = new SoundSrc(ballHit);
    wallCollideSnd = new SoundSrc(wallCollide);
    menuSoundSnd = new SoundSrc(menuSound);
    ballHitSnd->SetupDevice();
    wallCollideSnd->SetupDevice();
    menuSoundSnd->SetupDevice();
    //m_sounds["Collide.wav"] = sCollide;
    
    m_sounds[ballHit] = ballHitSnd;
    m_sounds[wallCollide] = wallCollideSnd;
    m_sounds[menuSound] = menuSoundSnd;
}

void AudioMgr::update()
{
    m_currentTime = SDL_GetTicks();
    if(m_currentTime > m_lastTime + 1000)
    {
        for(auto& sound: m_sounds)
        {
            sound.second->StopSound();
        }
        m_lastTime = m_currentTime;
    }
}

void AudioMgr::Play(std::string soundPath)
{
    m_sounds[soundPath]->PlaySound();
}

AudioMgr::~AudioMgr()
{
   // if(sCollide){
   // delete sCollide;
   // }
    if(ballHitSnd) delete ballHitSnd;
    if(wallCollideSnd) delete wallCollideSnd;
}
