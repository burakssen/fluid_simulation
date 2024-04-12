#pragma once

#include <raylib.h>
#include <raymath.h>

#include <iostream>

class Particle
{
public:
    Particle(Vector2 position);

    void Draw();

    void Update();
    void SetPosition(Vector2 position);
    void SetPreviousPosition(Vector2 previousPosition);
    void SetVelocity(Vector2 velocity);
    void SetColor(Color color);

    Vector2 GetPosition() const { return m_position; }
    Vector2 GetPreviousPosition() const { return m_previousPosition; }
    Vector2 GetVelocity() const { return m_velocity; }
    Color GetColor() const { return m_color; }

private:
    Vector2 m_position = Vector2Zero();
    Vector2 m_previousPosition = Vector2Zero();
    Vector2 m_velocity = Vector2Zero();
    Color m_color = {40, 176, 255, 255};
};

// hsva(202.047, 198%, 58%, 1)
// hsva(0, 198%, 51%, 1)