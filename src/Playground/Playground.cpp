#include "Playground.h"

Playground::Playground()
{
    this->m_simulation = &Simulation::GetInstance();
    this->m_simulation->InstantiateParticles();
}

Playground::~Playground()
{
}

Playground &Playground::GetInstance()
{
    static Playground instance;
    return instance;
}

void Playground::Update(float deltaTime)
{
    this->m_simulation->Update(deltaTime * 10.0f);
}

void Playground::Draw()
{
    this->m_simulation->Draw();
}

void Playground::HandleInput()
{
    this->m_simulation->HandleInput();
}