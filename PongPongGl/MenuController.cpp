#include "MenuController.h"
#include <iostream>

MenuController::MenuController()
{
}

void MenuController::init(){
    m_index = 0;
}

void MenuController::addUIElement(GameObject* me)
{
    m_mUI.emplace_back(me);
}

void MenuController::refresh()
{
    for(auto& ui : m_mUI)
    {
        ui->getComponent<MenuElement>().setIsSelected(false);
        ui->getComponent<MenuElement>().setText(ui->getComponent<MenuElement>().m_text);
    }
    m_mUI[m_index]->getComponent<MenuElement>().setIsSelected(true);
    m_mUI[m_index]->getComponent<MenuElement>().setText(m_mUI[m_index]->getComponent<MenuElement>().m_text);
           // std::cout << "HERE AFTERREFEE" << std::endl;
}

void MenuController::update()
{
    if(App::glb_event.type == SDL_KEYDOWN)
    {
        //std::cout << "HERE" << std::endl;
        auto& recCode = App::glb_event.key.keysym.sym;
        switch(recCode)
        {
            case SDLK_w:
            case SDLK_UP:
            if(!m_isMenuDisabled){
                App::glb_audioMgr.Play(AudioMgr::menuSound);
               // std::cout << "HERE" << std::endl;
                m_index++;
                if(m_index>=m_mUI.size()){
                    m_index = 0;
                }
                refresh();
                }
                break;
            case SDLK_s:
            case SDLK_DOWN:
                if(!m_isMenuDisabled){
                App::glb_audioMgr.Play(AudioMgr::menuSound);
                m_index--;
                if(m_index < 0)
                {
                    m_index = m_mUI.size()-1;
                }
                refresh();
                }
                break;
            case SDLK_RETURN:
            case SDLK_RETURN2:
                if(!m_isMenuDisabled){
                App::glb_audioMgr.Play(AudioMgr::menuSound);
                action();
                }
                break;
            case SDLK_m:
                if(App::glb_states.size()<2){
                MenuGameState* menuGmState(new MenuGameState(App::glb_appPtr));
                std::unique_ptr<State> scene2 {menuGmState};
                App::glb_states.top()->pause();
                App::glb_states.push(std::move(scene2));
                }
        }
    }
}

void MenuController::action()
{
            //    std::cout << "ENTERPRESSED" << std::endl;
    m_mUI[m_index]->getComponent<MenuElement>().act();
    m_isMenuDisabled = true;
}
