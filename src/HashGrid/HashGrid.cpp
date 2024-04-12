#include "HashGrid.h"

HashGrid::HashGrid(float cellSize)
{
    this->m_cellSize = cellSize;
}

HashGrid::~HashGrid()
{
}

void HashGrid::Initialize(std::vector<std::shared_ptr<Particle>> &particles)
{
    this->m_particles = particles;
}

void HashGrid::ClearHashMap()
{
    this->m_hashMap.clear();
}

int HashGrid::GetGridHashFromPos(Vector2 position)
{
    int x = (int)position.x / this->m_cellSize;
    int y = (int)position.y / this->m_cellSize;

    return this->CellIndexToHash(x, y);
}

int HashGrid::CellIndexToHash(int x, int y)
{
    int hash = (x * this->m_prime1) ^ (y * this->m_prime2);
    hash = hash % this->m_hashMapSize;

    return hash;
}

void HashGrid::MapParticlesToCell()
{
    for (int i = 0; i < this->m_particles.size(); i++)
    {
        Vector2 pos = this->m_particles[i]->GetPosition();
        int hash = this->GetGridHashFromPos(pos);

        auto entries = this->m_hashMap.find(hash);
        if (entries == this->m_hashMap.end())
        {
            std::vector<std::shared_ptr<Particle>> particles;
            particles.push_back(this->m_particles[i]);
            this->m_hashMap.insert({hash, particles});
            continue;
        }

        entries->second.push_back(this->m_particles[i]);
    }
}

std::vector<std::shared_ptr<Particle>> HashGrid::GetContentOffCell(int hash)
{
    auto entries = this->m_hashMap.find(hash);

    if (entries == this->m_hashMap.end())
    {
        std::vector<std::shared_ptr<Particle>> particles;
        return particles;
    }

    return entries->second;
}

std::vector<std::shared_ptr<Particle>> HashGrid::GetNeighborOfParticle(std::shared_ptr<Particle> particle)
{
    std::vector<std::shared_ptr<Particle>> neighbors;
    Vector2 pos = particle->GetPosition();

    int particleGridX = (int)pos.x / this->m_cellSize;
    int particleGridY = (int)pos.y / this->m_cellSize;

    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            int gridX = particleGridX + x;
            int gridY = particleGridY + y;

            int hash = this->CellIndexToHash(gridX, gridY);
            std::vector<std::shared_ptr<Particle>> content = this->GetContentOffCell(hash);

            neighbors.insert(neighbors.end(), content.begin(), content.end());
        }
    }

    return neighbors;
}

std::vector<std::shared_ptr<Particle>> HashGrid::GetNeighborOfParticleIndex(int i)
{
    std::vector<std::shared_ptr<Particle>> neighbors;
    Vector2 pos = this->m_particles[i]->GetPosition();

    int particleGridX = (int)pos.x / this->m_cellSize;
    int particleGridY = (int)pos.y / this->m_cellSize;

    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            int gridX = particleGridX + x;
            int gridY = particleGridY + y;

            int hash = this->CellIndexToHash(gridX, gridY);
            std::vector<std::shared_ptr<Particle>> content = this->GetContentOffCell(hash);

            neighbors.insert(neighbors.end(), content.begin(), content.end());
        }
    }

    return neighbors;
}