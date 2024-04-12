#include "App.h"

App::App()
{
    InitWindow(this->m_width, this->m_height, this->m_title.c_str());
    SetTargetFPS(60);

    this->m_playground = &Playground::GetInstance();
    this->m_lastFrameTime = 0.016f;
}

App::~App()
{
    CloseWindow();
}

App &App::GetInstance()
{
    static App instance;
    return instance;
}

void App::Update()
{
    float currentTime = GetTime();
    float deltaTime = currentTime - this->m_lastFrameTime;
    this->m_lastFrameTime = currentTime;
    this->m_playground->Update(deltaTime);
}

void App::Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    this->m_playground->Draw();

    DrawFPS(10, 10);
    EndDrawing();
}

void App::Run()
{
    while (!WindowShouldClose())
    {
        this->HandleInput();
        this->Update();
        this->Draw();
    }
}

void App::HandleInput()
{
    this->m_playground->HandleInput();
}