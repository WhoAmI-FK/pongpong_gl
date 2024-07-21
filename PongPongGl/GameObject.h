#pragma once
#include <vector>
#include <memory>
#include <array>
#include "Constants.h"
#include "GameObject.h"

class GameObjectMgr;
class GameObject;
class Component;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static_assert (std::is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

class Component
{
public:
    GameObject* m_gObjPtr;

    virtual void init(){}
    virtual void update(){}
    virtual void render(){}
    virtual ~Component(){}
};


class GameObject
{
private:
	GameObjectMgr& m_mgr;
    bool m_active = true;
    std::vector<std::unique_ptr<Component>> m_components;

    ComponentArray m_componentArray;
    ComponentBitSet m_componentBitset;
    GroupBitset m_groupBitset;
public:
    GameObject(GameObjectMgr& mgr);

    void update();
    void render();
    bool isActive() const;
    void disable();
    void enable();


    bool hasGroup(Group group);
    void addGroup(Group group);
    void delGroup(Group group);

    template <typename T> bool hasComponent() const
	{
		return m_componentBitset[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->m_gObjPtr = this;
		std::unique_ptr<Component>uPtr { c };
		m_components.emplace_back(std::move(uPtr));

		m_componentArray[getComponentTypeID<T>()] = c;
		m_componentBitset[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

    template<typename T> T& getComponent() const
	{
		auto ptr(m_componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class GameObjectMgr
{
private:
	std::vector<std::unique_ptr<GameObject>> m_gObjs;
	std::array<std::vector<GameObject*>, maxGroups> m_grGObjs;
public:
	void update();
	void render();
	// void refresh
	void AddToGroup(GameObject* obj, Group group);
	std::vector<GameObject*>& getGroup(Group group);
	GameObject& addGObj();
};
