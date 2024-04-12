#pragma once

#include <map>
#include <vector>
#include <memory>

#include "Particle/Particle.h"

class HashGrid
{
public:
    HashGrid(float cellSize);
    ~HashGrid();
    void Initialize(std::vector<std::shared_ptr<Particle>> &particles);
    void ClearHashMap();

    int GetGridHashFromPos(Vector2 position);
    int CellIndexToHash(int x, int y);
    void MapParticlesToCell();
    std::vector<std::shared_ptr<Particle>> GetContentOffCell(int hash);

    std::vector<std::shared_ptr<Particle>> GetNeighborOfParticleIndex(int i);
    std::vector<std::shared_ptr<Particle>> GetNeighborOfParticle(std::shared_ptr<Particle> particle);

private:
    float m_cellSize = 0.0f;
    std::map<int, std::vector<std::shared_ptr<Particle>>> m_hashMap;
    int m_hashMapSize = 10000000;
    long m_prime1 = 6614058611;
    long m_prime2 = 7528850467;

    std::vector<std::shared_ptr<Particle>> m_particles;
};
