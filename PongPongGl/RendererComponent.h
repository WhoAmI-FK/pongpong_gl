#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include "Renderable.h"
#include "Constants.h"
#include "Application.h"

#include <SDL.h>
#include <cmath>
/*
inline ComponentID getNewRenderableTypeID()
{
	static RenderableID lastID = 0u;
	return lastID++;
}

template <typename T> inline RenderableID getRenderableTypeID() noexcept
{
	static_assert (std::is_base_of<Renderable, T>::value, "");
	static RenderableID typeID = getNewComponentTypeID();
	return typeID;
}
// TBD LATER
*/
class Renderable;

class RendererComponent : public Component
{
private:
    std::vector<std::unique_ptr<Renderable>> m_renderables;
    // TBD LATER
    //RenderableArray m_renderableArray;
    //RenderableBitSet m_renderableBitset;
public:
    TransformComponent* m_transformPtr; // TODO move to private, but then need to fix in renderable priv context
    RendererComponent() = default;
    ~RendererComponent();

	template <typename T, typename... TArgs>
	T& addRenderable(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->_renderer = this;
		std::unique_ptr<Renderable>uPtr { c };
		m_renderables.emplace_back(std::move(uPtr));

    // TBD LATER
	//	m_renderableArray[getRenderableTypeID<T>()] = c;
	//	m_renderableBitset[getRenderableTypeID<T>()] = true;

		c->init();
		return *c;
	}

    void init() override;
    void update() override;
    void render() override;
};

class Renderable
{
public:
    RendererComponent* _renderer;
    virtual void init(){}
    virtual void update(){}
    virtual void render(){}
    virtual ~Renderable(){}
};

class RenderableRec : public Renderable
{
    // we will be assuming that every object has transform for now
private:
    SDL_Rect m_srcRect;
public:
    void init() override {
        m_srcRect.x = m_srcRect.y = 0;
        m_srcRect.w = _renderer->m_transformPtr->m_width;
        m_srcRect.h = _renderer->m_transformPtr->m_height;
    }
    void update() override {
        m_srcRect.x = _renderer->m_transformPtr->m_position.x;
        m_srcRect.y = _renderer->m_transformPtr->m_position.y;
        m_srcRect.w = _renderer->m_transformPtr->m_width;
        m_srcRect.h = _renderer->m_transformPtr->m_height;
    }
    void render() override {
        SDL_SetRenderDrawColor(App::glb_renderer, 255,255,255,255);
	    SDL_RenderFillRect(App::glb_renderer, &m_srcRect);
	    SDL_SetRenderDrawColor(App::glb_renderer, 0, 0, 0, 255);
    }
};

class RenderableCircle : public Renderable
{
private:
    int m_radius;
    float x;
    float y;
public:
    void init() override {
        x = y = 0;
        m_radius = _renderer->m_transformPtr->m_radius;
    }
    void update() override {
        x = _renderer->m_transformPtr->m_position.x;
        y = _renderer->m_transformPtr->m_position.y;
        m_radius = _renderer->m_transformPtr->m_radius;
    }
    void render() override {

    for (double dy = 1; dy <= m_radius; dy += 1.0)
	{
		double dx = floor(sqrt((2.0 * m_radius * dy) - (dy * dy)));
		float cx = x;
		double x = cx - dx;
		float cy = y;
		SDL_SetRenderDrawColor(App::glb_renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(App::glb_renderer, static_cast<int>(cx - dx), static_cast<int>(cy + dy - m_radius), static_cast<int>(cx + dx), static_cast<int>(cy + dy - m_radius));
		SDL_RenderDrawLine(App::glb_renderer, static_cast<int>(cx - dx), static_cast<int>(cy - dy + m_radius), static_cast<int>(cx + dx), static_cast<int>(cy - dy + m_radius));
		SDL_SetRenderDrawColor(App::glb_renderer, 0, 0, 0, 255);
	}

    }
};
