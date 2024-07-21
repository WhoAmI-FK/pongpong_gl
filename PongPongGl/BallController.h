#pragma once

#include "Application.h"
#include "TransformComponent.h"
#include "Constants.h"
#include "Sound.h"

bool Collision(GameObject* g1, GameObject* g2);

//SoundSrc* sCollide;//("Collide.wav"); // change to sound mgr / or component

class BallController : public Component
{
public:
    TransformComponent *m_transform;
    GameObject* player_l;
    GameObject* player_r;
    int m_hits;

    BallController(GameObject* playerLeft, GameObject* playerRight)
    : player_l(playerLeft), player_r(playerRight)
    {

    }

    void init() override
    {
        m_transform = &m_gObjPtr->getComponent<TransformComponent>();
        m_transform->m_velocity.y = 1;
        m_transform->m_velocity.x = 1;
        m_hits = 0;
        //sCollide = new SoundSrc("Collide.wav");
        //sCollide->SetupDevice();
    }

    void update() override
    {
        //std::cout <<"BALLSPEED:X: " << m_transform->m_velocity.x * m_transform->m_speed << std::endl;
        //std::cout <<"BALLSPEED:Y: " << m_transform->m_velocity.y * m_transform->m_speed << std::endl;
        if(m_transform->m_position.y < 0) {
            App::glb_audioMgr.Play(AudioMgr::wallCollide);
            VerticalBounce();
            m_transform->m_position.y = 0;
        }
        else if(m_transform->m_position.y + m_transform->m_radius > SCREEN_HEIGHT)
        {
            App::glb_audioMgr.Play(AudioMgr::wallCollide);
            VerticalBounce();
            m_transform->m_position.y = static_cast<float>(SCREEN_HEIGHT - m_transform->m_radius);
        }

       // if(m_transform->m_position.x < player_l->getComponent<TransformComponent>().m_width + m_transform->m_radius  +  player_l->getComponent<TransformComponent>().m_position.x)
        if(Collision(this->m_gObjPtr, player_l))
        {
            App::glb_audioMgr.Play(AudioMgr::ballHit);
            //sCollide->PlaySound();
            float LPY = player_l->getComponent<TransformComponent>().m_position.y;
            if(m_transform->m_position.y >= LPY &&
               m_transform->m_position.y <= LPY + player_l->getComponent<TransformComponent>().m_height)
            {
                HorizontalBounce(true);
                m_transform->m_position.x = static_cast<float>(player_l->getComponent<TransformComponent>().m_width + m_transform->m_radius + player_l->getComponent<TransformComponent>().m_position.x);
            }
            m_hits++;
        }

        //if(m_transform->m_position.x > player_r->getComponent<TransformComponent>().m_position.x - m_transform->m_radius)
        if(Collision(this->m_gObjPtr, player_r))
        {
                    //    sCollide->PlaySound();
            App::glb_audioMgr.Play(AudioMgr::ballHit);
            float RPY = player_r->getComponent<TransformComponent>().m_position.y;
            if(m_transform->m_position.y >= RPY &&
               m_transform->m_position.y <= RPY + player_r->getComponent<TransformComponent>().m_height)
            {
                HorizontalBounce(false);
                m_transform->m_position.x = static_cast<float>(player_r->getComponent<TransformComponent>().m_position.x - m_transform->m_radius);
            }
            m_hits++;
        }

            if(m_hits==3)
            {
                std::cout << "Speed increased: " << m_transform->m_speed << std::endl;
                m_transform->increaseSpeedBy(m_transform->m_speed * 0.2);
                m_hits = 0;
            }
        //static int currentTime   = SDL_GetTicks();
        //static int lastTime      = SDL_GetTicks();
        //currentTime = SDL_GetTicks();
        //if (currentTime > lastTime + 1000) {
        //->StopSound();
        //lastTime = currentTime;
        //}
    }

    void VerticalBounce()
    {
        m_transform->m_velocity.y = -m_transform->m_velocity.y;
    }

    void HorizontalBounce(bool isLPadle)
    {
        const float PBOUNCE_MULT = 10.f;
        m_transform->m_velocity.x = -m_transform->m_velocity.x;
        if(isLPadle){
            // Process Left Paddle hits
            float lpaddle_center = player_l->getComponent<TransformComponent>().m_position.y + player_l->getComponent<TransformComponent>().m_height / 2;
            m_transform->m_velocity.y = (m_transform->m_position.y - lpaddle_center) / player_l->getComponent<TransformComponent>().m_height / 2 * m_transform->m_speed * PBOUNCE_MULT;

        }else{
            // Process Right Paddle hits
            float rpaddle_center = player_r->getComponent<TransformComponent>().m_position.y + player_r->getComponent<TransformComponent>().m_height / 2;
            m_transform->m_velocity.y = (m_transform->m_position.y - rpaddle_center) / player_r->getComponent<TransformComponent>().m_height / 2 * m_transform->m_speed * PBOUNCE_MULT;
        }
    }
};


// Temporary Solution
// TODO - Later move this logic to some ColliderComponent + Collision and etc.


bool Collision(GameObject* g1, GameObject* g2)
{
    auto& g1_t1 = g1->getComponent<TransformComponent>();
    auto& g2_t2 = g2->getComponent<TransformComponent>();

    if(g1_t1.m_position.x + g1_t1.m_width + (g1_t1.m_radius) >= g2_t2.m_position.x &&
       g2_t2.m_position.x + g2_t2.m_width + (g2_t2.m_radius) >= g1_t1.m_position.x &&
       g1_t1.m_position.y + g1_t1.m_height + (g1_t1.m_radius) >= g2_t2.m_position.y &&
       g2_t2.m_position.y + g2_t2.m_height + (g2_t2.m_radius) >= g1_t1.m_position.y)
    {
        return true;
    }
    return false;
}
