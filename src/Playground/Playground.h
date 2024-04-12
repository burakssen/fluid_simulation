#pragma once

#include <iostream>

#include <raylib.h>

#include "Simulation/Simulation.h"

class Playground
{
    Playground();

public:
    ~Playground();
    static Playground &GetInstance();
    void Update(float deltaTime);
    void Draw();
    void HandleInput();

private:
    Simulation *m_simulation = nullptr;
};