#include "TextElement.h"
#include "Application.h"
#include <iostream>

TextElement::TextElement(int xP, int yP, std::string text,
                         std::string font, int fSize, SDL_Color& color)
    : m_text(text), m_font(font), m_color(color), m_fsize(fSize)
{
    m_position.x = xP;
    m_position.y = yP;

    // Load font here
    m_ttfFont = TTF_OpenFont(font.c_str(), fSize);
    setText(text);
}

TextElement::~TextElement(){}

void TextElement::setText(std::string text)
{
    SDL_Surface* surface = TTF_RenderText_Blended(m_ttfFont, text.c_str(), m_color);
    m_texture = SDL_CreateTextureFromSurface(App::glb_renderer, surface);
    SDL_FreeSurface(surface);

   // std::cout << "UPDATED: " << m_text << std::endl;

    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_position.w, &m_position.h);
}

// void setFont - later add this func

void TextElement::render()
{
    SDL_RenderCopy(App::glb_renderer, m_texture, nullptr, &m_position);
}


MenuElement::~MenuElement(){}

MenuElement::MenuElement(int xP, int yP, std::string text,
                              std::string font, int fSize, SDL_Color& color,
                              SDL_Color& hoverColor,
                              bool isSelected)
    : m_text(text), m_font(font), m_color(color), m_hoverColor(hoverColor),
      m_fsize(fSize), m_isSelected(isSelected)
{
    m_position.x = xP;
    m_position.y = yP;

    m_ttfFont = TTF_OpenFont(font.c_str(), fSize);
    setText(text);
}

void MenuElement::setIsSelected(bool isSelected)
{
    m_isSelected = isSelected;
}
void MenuElement::setText(std::string text)
{
   // std::cout << "TEXTSET:" << m_text << ";isUP:" << m_isSelected << std::endl;
     SDL_Surface* surface;
    if(m_isSelected){
        surface = TTF_RenderText_Blended(m_ttfFont, text.c_str(), m_hoverColor);
    }
    else
    {
       surface = TTF_RenderText_Blended(m_ttfFont, text.c_str(), m_color);
    }
    m_texture = SDL_CreateTextureFromSurface(App::glb_renderer, surface);
    SDL_FreeSurface(surface);


    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_position.w, &m_position.h);
}

void MenuElement::render()
{
    SDL_RenderCopy(App::glb_renderer, m_texture, nullptr, &m_position);
}

void MenuElement::setAction(Action* action)
{
    m_action.reset(std::move(action));
}

void MenuElement::act()
{
    m_action->act();
}
