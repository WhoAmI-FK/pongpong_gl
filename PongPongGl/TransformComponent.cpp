#include "TransformComponent.h"
#include <algorithm>

TransformComponent::TransformComponent(ObjectTag ot) :
     m_height(32), m_width(32),
     m_scale(1),   m_speed(3),
     m_tag(ot),
     m_radius(0) // only for circle-like objs
{
    m_position.Zero();
}

TransformComponent::TransformComponent(int scale, ObjectTag ot) :
    m_height(32), m_width(32),
    m_scale(scale), m_speed(3),
    m_radius(0), m_tag(ot)
{
    m_position.Zero();
}

TransformComponent::TransformComponent(float x, float y, ObjectTag ot) :
     m_height(32), m_width(32),
     m_scale(1),   m_speed(3),
     m_radius(0), m_tag(ot)
{
    m_position.Zero();
    m_position.x = x;
    m_position.y = y;
}

TransformComponent::TransformComponent(float x, float y, int h, int w, int scale, ObjectTag ot)
    : m_height(h), m_width(w),
      m_scale(scale), m_speed(10),
      m_radius(0), m_tag(ot)
{
    m_position.Zero();
    m_position.x = x;
    m_position.y = y;
}

TransformComponent::TransformComponent(float x, float y, int r, ObjectTag ot)
    : m_radius(r), m_speed(DEF_BALL_SPEED), m_tag(ot), m_height(0), m_width(0)
{
    m_position.Zero();
    m_position.x = x;

    m_position.y = y;
}

void TransformComponent::init()
{
    m_velocity.Zero();
}

void TransformComponent::update()
{
	m_position.x += static_cast<int>(m_velocity.x * m_speed);
	m_position.y += static_cast<int>(m_velocity.y * m_speed);
    if(m_tag==ObjectTag::PLAYER){
    // Temporary solution, but preferable to refactor this place later
    m_position.y = std::clamp<float>(m_position.y, 0, SCREEN_HEIGHT - m_height);
    }
}

void TransformComponent::increaseSpeedBy(float inc)
{
    m_speed += inc;
}
