#include "Particle.h"

Particle::Particle(Vector2 position)
{
    this->m_position = position;
    this->m_previousPosition = position;
}

void Particle::Update()
{
    Color color = this->m_color;
    float speed = Vector2Length(this->m_velocity);

    if (speed > 32.0f)
        color = {255, 0, 0, 255};
    else if (speed > 30.0f)
        color = {255, 0, 88, 255};
    else if (speed > 25.0f)
        color = {255, 2, 146, 255};
    else if (speed > 20.0f)
        color = {248, 62, 201, 255};
    else if (speed > 15.0f)
        color = {209, 106, 241, 255};
    else if (speed > 10.0f)
        color = {158, 136, 255, 255};
    else if (speed > 5.0f)
        color = {102, 158, 255, 255};
    else
        color = {41, 173, 255, 255};

    this->m_color = color;
}

void Particle::SetPosition(Vector2 position)
{
    this->m_position = position;
}

void Particle::SetPreviousPosition(Vector2 previousPosition)
{
    this->m_previousPosition = previousPosition;
}

void Particle::SetVelocity(Vector2 velocity)
{
    this->m_velocity = velocity;
}

void Particle::SetColor(Color color)
{
    this->m_color = color;
}

void Particle::Draw()
{
    DrawCircleV(this->m_position, 5.0f, this->m_color);
}