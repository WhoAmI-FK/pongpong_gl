#include "GameObject.h"

GameObject::GameObject(GameObjectMgr& mgr) : m_mgr(mgr)
{}

void GameObject::update()
{
	for (auto& c : m_components)
        c->update();
}

void GameObject::render()
{
	for (auto& c : m_components)
        c->render();
}

bool GameObject::isActive() const
{
    return m_active;
}

void GameObject::disable()
{
    m_active = false;
}

void GameObject::enable()
{
    m_active = true;
}

bool GameObject::hasGroup(Group group)
{
    return m_groupBitset[group];
}

void GameObject::addGroup(Group group)
{
    m_groupBitset[group] = true;
    m_mgr.AddToGroup(this, group);
}

void GameObject::delGroup(Group group)
{
    m_groupBitset[group] = false;
}

/*---------------------------------------------------*/

void GameObjectMgr::update()
{
    for(auto& obj: m_gObjs) obj->update();
}

void GameObjectMgr::render()
{
    for(auto& obj: m_gObjs) obj->render();
}

void GameObjectMgr::AddToGroup(GameObject* obj, Group group) {
    m_grGObjs[group].emplace_back(obj);
}

std::vector<GameObject*>& GameObjectMgr::getGroup(Group group)
{
    return m_grGObjs[group];
}

GameObject& GameObjectMgr::addGObj(){
    GameObject* obj = new GameObject(*this);
    std::unique_ptr<GameObject> uPtr{obj};
    m_gObjs.emplace_back(std::move(uPtr));
    return *obj;
}
