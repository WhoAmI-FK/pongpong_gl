#include "RendererComponent.h"
#include "Application.h"

RendererComponent::~RendererComponent()
{

}

void RendererComponent::init() {
    m_transformPtr = &m_gObjPtr->getComponent<TransformComponent>();
}

void RendererComponent::update() {
    for(auto& renderable: m_renderables)
    {
        renderable->update();
    }
}

void RendererComponent::render()
{
    for(auto& renderable: m_renderables)
    {
        renderable->render();
    }
}
