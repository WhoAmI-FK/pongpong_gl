#pragma once

#include "GameObject.h"
#include "Application.h"

#include <SDL_ttf.h>
#include <SDL.h>
#include <string>


// Another solution is to inherit Renderable class and move
// everything to RendererComponent logic, but for now
// I will just separate text and geometry, later
// if needed can be moved to RendererComponent

class TextElement : public Component
{
public:
    TextElement(int xP, int yP, std::string text,
                std::string font, int fSize, SDL_Color& color);
    ~TextElement();

    void setText(std::string text);
    void render() override;

private:
    SDL_Rect m_position;
    std::string m_text;
    std::string m_font;
    SDL_Color m_color;
    SDL_Texture* m_texture;
    TTF_Font* m_ttfFont;
    int m_fsize;
};
// Simple dull solution, but works :)
class ScoreTracker : public Component
{
public:
    void updateScore(int score)
    {
        this->m_gObjPtr->getComponent<TextElement>().setText(std::to_string(score));
    }
    //TextElement* m_te;
};

class Action {
public:
    virtual void act() {};
};

class ExitAction : public Action
{
public:
    void act(){
        App::glb_isRunning = false;
    }
};


class PlayAction : public Action
{
public:
    void act()
    {
        App::glb_states.pop();
//        PlayerChoiceState* plChState(new PlayerChoiceState(App::glb_appPtr));
//        std::unique_ptr<State> scene3 {plChState};
//        App::glb_states.push(std::move(scene3));
        App::glb_states.top()->init();
    }
};

class PlayerChoiceAction : public Action
{
public:

    void act()
    {
        App::glb_states.pop(); // PlayerChoice Scene pop
        App::glb_states.top()->init(); // main game init
    }
};

class MenuElement : public Component
{
public:
    MenuElement(int xP, int yP, std::string text,
                std::string font, int fSize, SDL_Color& color,
                SDL_Color& hoverColor, bool isSelected);
    ~MenuElement();

    void setText(std::string text);
    std::string m_text; // make private back later
    void render() override;
    void setAction(Action* action);
    void act();
    void setIsSelected(bool isSelected);
private:
    SDL_Rect m_position;
    std::string m_font;
    SDL_Color m_color;
    SDL_Color m_hoverColor;
    SDL_Texture* m_texture;
    TTF_Font* m_ttfFont;
    int m_fsize;
    bool m_isSelected;
    std::unique_ptr<Action> m_action;
};
