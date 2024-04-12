#pragma once

#include <vector>
#include <memory>
#include <thread>
#include <algorithm>
#include <execution>

#include <raylib.h>
#include <raymath.h>

#include "Particle/Particle.h"
#include "HashGrid/HashGrid.h"

class Simulation
{
private:
    Simulation();

public:
    static Simulation &GetInstance();
    void Update(float deltaTime);
    void Draw();

    void InstantiateParticles();
    void PredictPositions(float deltaTime);
    void ComputeNextVelocity(float deltaTime);
    void WorldBoundary();
    void NeighborSearch();

    void ApplyGravity(float deltaTime);
    void DoubleDensityRelaxation(float deltaTime);
    void Viscosity(float deltaTime);

    void HandleInput();

private:
    std::vector<std::shared_ptr<Particle>> m_particles;
    float m_amountOfParticles = 2000;
    float m_velocityDamping = 0.99f;
    std::shared_ptr<HashGrid> m_hashGrid = nullptr;
    Vector2 m_gravity = {0.0f, 1.0f};
    float m_restDensity = 10.0f;
    float m_K_Near = 3.0f;
    float m_K = 0.5f;
    float m_interactionRadius = 30.0f;
    // Viscosity parameters
    float m_sigma = -0.5f;
    float m_beta = -0.0005f;
};
