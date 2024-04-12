#pragma once

#include <string>

#include <raylib.h>

#include "Playground/Playground.h"

class App
{
    App();

public:
    ~App();
    static App &GetInstance();

    void Update();
    void Draw();
    void HandleInput();
    void Run();

private:
    float m_width = 1280.0f;
    float m_height = 720.0f;

    std::string m_title = "Fluid Simulation";

    Playground *m_playground = nullptr;

    float m_lastFrameTime = 0.0f;
};