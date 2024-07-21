#pragma once
#include "GameObject.h"
#include "Vector2D.h"

enum ObjectTag{
    PLAYER,
    BALL,
    DEFAULT
};

class TransformComponent : public Component
{
public:
    Vector2D m_position;
    Vector2D m_velocity;

    int m_height;
    int m_width;
    int m_scale;
    float m_speed;
    int m_radius;
    // Temporary solution
    ObjectTag m_tag;

    TransformComponent(ObjectTag ot);
    TransformComponent(int scale, ObjectTag ot);
    TransformComponent(float x, float y, ObjectTag ot);
    TransformComponent(float x, float y, int h, int w, int scale, ObjectTag ot);
    TransformComponent(float x, float y, int r, ObjectTag ot);

    void init() override;
    void update() override;
    void increaseSpeedBy(float inc);
    

};
