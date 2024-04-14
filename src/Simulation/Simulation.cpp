#include "Simulation.h"

Simulation::Simulation()
{
    this->m_hashGrid = std::make_shared<HashGrid>(this->m_interactionRadius);
}

Simulation &Simulation::GetInstance()
{
    static Simulation instance;
    return instance;
}

void Simulation::Update(float deltaTime)
{

    this->ApplyGravity(deltaTime);
    this->Viscosity(deltaTime);
    this->PredictPositions(deltaTime);
    this->NeighborSearch();
    this->DoubleDensityRelaxation(deltaTime);
    this->WorldBoundary();
    this->ComputeNextVelocity(deltaTime);

    for (int i = 0; i < this->m_particles.size(); i++)
    {
        this->m_particles[i]->Update();
    }
}

void Simulation::Draw()
{
    for (int i = 0; i < this->m_particles.size(); i++)
    {
        this->m_particles[i]->Draw();
    }
}

void Simulation::PredictPositions(float deltaTime)
{
    for (int i = 0; i < this->m_particles.size(); i++)
    {
        this->m_particles[i]->SetPreviousPosition(this->m_particles[i]->GetPosition());
        Vector2 positionDelta = Vector2Scale(this->m_particles[i]->GetVelocity(), deltaTime * this->m_velocityDamping);
        this->m_particles[i]->SetPosition(Vector2Add(this->m_particles[i]->GetPosition(), positionDelta));
    }
}

void Simulation::ComputeNextVelocity(float deltaTime)
{
    for (int i = 0; i < this->m_particles.size(); i++)
    {
        Vector2 velocity = Vector2Scale(Vector2Subtract(this->m_particles[i]->GetPosition(), this->m_particles[i]->GetPreviousPosition()), 1.0f / deltaTime);
        this->m_particles[i]->SetVelocity(velocity);
    }
}

void Simulation::InstantiateParticles()
{
    float xParticles = sqrt(this->m_amountOfParticles);
    float yParticles = xParticles;

    float offsetBetweenParticles = 10.0f;

    float offsetX = GetScreenWidth() / 2 - (xParticles * offsetBetweenParticles) / 2 + GetRandomValue(-GetScreenWidth() / 2, GetScreenWidth() / 2);
    float offsetY = GetScreenHeight() / 2 - (yParticles * offsetBetweenParticles) / 2 + GetRandomValue(-100, 100);

    for (int i = 0; i < xParticles; i++)
    {
        for (int j = 0; j < yParticles; j++)
        {
            Vector2 position = {i * offsetBetweenParticles + offsetX,
                                j * offsetBetweenParticles + offsetY};

            std::shared_ptr<Particle> particle = std::make_shared<Particle>(position);
            Vector2 velocity = {(float)GetRandomValue(-100, 100), (float)GetRandomValue(-100, 100)};
            // particle->SetVelocity(velocity);
            this->m_particles.push_back(particle);
        }
    }

    this->m_hashGrid.get()->Initialize(this->m_particles);
}

void Simulation::NeighborSearch()
{
    this->m_hashGrid.get()->ClearHashMap();
    this->m_hashGrid.get()->MapParticlesToCell();
}

void Simulation::WorldBoundary()
{
    for (int i = 0; i < this->m_particles.size(); i++)
    {
        Vector2 pos = this->m_particles[i]->GetPosition();
        Vector2 prevPos = this->m_particles[i]->GetPreviousPosition();

        if (pos.x < 0)
        {
            this->m_particles[i]->SetPosition({0, pos.y});
            this->m_particles[i]->SetPreviousPosition({0, prevPos.y});
        }

        if (pos.x > GetScreenWidth())
        {
            this->m_particles[i]->SetPosition({(float)GetScreenWidth(), pos.y});
            this->m_particles[i]->SetPreviousPosition({(float)GetScreenWidth(), prevPos.y});
        }

        if (pos.y < 0)
        {
            this->m_particles[i]->SetPosition({pos.x, 0});
            this->m_particles[i]->SetPreviousPosition({prevPos.x, 0});
        }

        if (pos.y > GetScreenHeight())
        {
            this->m_particles[i]->SetPosition({pos.x, (float)GetScreenHeight()});
            this->m_particles[i]->SetPreviousPosition({prevPos.x, (float)GetScreenHeight()});
        }
    }
}

void Simulation::ApplyGravity(float deltaTime)
{
    for (int i = 0; i < this->m_particles.size(); i++)
    {
        this->m_particles[i]->SetVelocity(Vector2Add(this->m_particles[i]->GetVelocity(), Vector2Scale(this->m_gravity, deltaTime)));
    }
}

void Simulation::DoubleDensityRelaxation(float deltaTime)
{

    for (int i = 0; i < this->m_particles.size(); i++)
    {
        float density = 0.0f;
        float nearDensity = 0.0f;

        std::vector<std::shared_ptr<Particle>> neighbors = this->m_hashGrid.get()->GetNeighborOfParticleIndex(i);
        Particle *particleA = this->m_particles[i].get();

        for (int j = 0; j < neighbors.size(); j++)
        {
            Particle *particleB = neighbors[j].get();

            if (particleA == particleB)
                continue;

            Vector2 rij = Vector2Subtract(particleB->GetPosition(), particleA->GetPosition());
            float q = Vector2Length(rij) / this->m_interactionRadius;

            if (q < 1)
            {
                density += (1 - q) * (1 - q);
                nearDensity += (1 - q) * (1 - q) * (1 - q);
            }
        }

        float pressure = this->m_K * (density - this->m_restDensity);
        float nearPressure = this->m_K_Near * nearDensity;

        Vector2 pressureForce = Vector2Zero();
        Vector2 nearPressureForce = Vector2Zero();
        Vector2 particleADisplacement = Vector2Zero();

        for (int j = 0; j < neighbors.size(); j++)
        {
            Particle *particleB = neighbors[j].get();

            if (particleA == particleB)
                continue;

            Vector2 rij = Vector2Subtract(particleB->GetPosition(), particleA->GetPosition());
            float q = Vector2Length(rij) / this->m_interactionRadius;

            if (q < 1.0f)
            {
                rij = Vector2Normalize(rij);
                float displacementTerm = deltaTime * deltaTime * (pressure * (1 - q) + nearPressure * (1 - q) * (1 - q));
                Vector2 D = Vector2Scale(rij, displacementTerm);

                particleB->SetPosition(Vector2Add(particleB->GetPosition(), Vector2Scale(D, 0.5f)));
                particleADisplacement = Vector2Subtract(particleADisplacement, Vector2Scale(D, 0.5f));
            }
        }

        particleA->SetPosition(Vector2Add(particleA->GetPosition(), particleADisplacement));
    }
}

void Simulation::Viscosity(float deltaTime)
{
    for (int i = 0; i < this->m_particles.size(); i++)
    {
        std::vector<std::shared_ptr<Particle>> neighbors = this->m_hashGrid.get()->GetNeighborOfParticleIndex(i);
        Particle *particleA = this->m_particles[i].get();

        for (int j = 0; j < neighbors.size(); j++)
        {
            Particle *particleB = neighbors[j].get();

            if (particleA == particleB)
                continue;

            Vector2 rij = Vector2Subtract(particleB->GetPosition(), particleA->GetPosition());
            float q = Vector2Length(rij) / this->m_interactionRadius;
            Vector2 velocityA = particleA->GetVelocity();
            Vector2 velocityB = particleB->GetVelocity();

            if (q < 1.0f)
            {
                rij = Vector2Normalize(rij);
                float u = Vector2DotProduct(rij, Vector2Subtract(velocityB, velocityA));

                if (u > 0.0f)
                {
                    float iTerm = deltaTime * (1 - q) * (this->m_sigma * u + this->m_beta * u * u);
                    Vector2 I = Vector2Scale(rij, iTerm);

                    particleA->SetVelocity(Vector2Subtract(particleA->GetVelocity(), Vector2Scale(I, 0.5f)));
                    particleB->SetVelocity(Vector2Add(particleB->GetVelocity(), Vector2Scale(I, 0.5f)));
                }
            }
        }
    }
}

void Simulation::HandleInput()
{
    if (IsKeyPressed(KEY_R))
    {
        this->m_particles.clear();
        this->InstantiateParticles();
    }
}