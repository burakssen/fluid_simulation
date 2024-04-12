#include "Particle.h"

Particle::Particle(Vector2 position)
{
    this->m_position = position;
    this->m_previousPosition = position;
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