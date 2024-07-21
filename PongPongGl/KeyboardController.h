// TODO MOVE TO CPP PART OF CODE
#pragma once

#include "Application.h"
#include "TransformComponent.h"
#include "RendererComponent.h"

class KeyboardController : public Component
{
public:
    KeyboardController(SDL_KeyCode UP, SDL_KeyCode DOWN)
    {
        UP_KEY = UP;
        DOWN_KEY = DOWN;
    }

    TransformComponent *m_transform;
    RendererComponent  *m_renderer;

    void init() override
    {
        m_transform = &m_gObjPtr->getComponent<TransformComponent>();
        m_renderer  = &m_gObjPtr->getComponent<RendererComponent>();
    }

    void update() override
    {
        if(App::glb_event.type == SDL_KEYDOWN)
        {

            auto& recCode = App::glb_event.key.keysym.sym;

            if(recCode == UP_KEY)
            {
                m_transform->m_velocity.y = -1;
            }
            else if(recCode == DOWN_KEY)
            {
                m_transform->m_velocity.y = 1;
            }


        }

        if(App::glb_event.type == SDL_KEYUP)
        {
            auto& recCode = App::glb_event.key.keysym.sym;

            if(recCode == UP_KEY || recCode == DOWN_KEY)
            {
                m_transform->m_velocity.y = 0;
            }
            else if(recCode==SDLK_ESCAPE)
            {
                App::glb_isRunning = false;
            }
        }
    }
private:
 SDL_KeyCode UP_KEY;
 SDL_KeyCode DOWN_KEY;
};
