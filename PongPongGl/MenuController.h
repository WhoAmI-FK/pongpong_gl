#pragma once

#include "Application.h"
#include "GameObject.h"
#include <memory>
#include <vector>
#include "TextElement.h"

class MenuController : public Component
{
public:
    MenuController();
    void init() override;
    void update() override;

    void addUIElement(GameObject* me);
    void refresh();
    void action();
    bool m_isMenuDisabled;
private:
    std::vector<GameObject*> m_mUI;
    int m_index;
};
