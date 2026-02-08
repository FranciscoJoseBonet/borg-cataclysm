#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "EntityManager.h"
#include "ResourceManager.h"

class LevelManager
{
private:
    EntityManager &entityManager;
    ResourceManager &resources;
    sf::Vector2f worldBounds;

    int currentLevel = 1;
    int enemiesKilled = 0;
    int enemiesToSpawn = 0;
    bool waveInProgress = false;

    std::mt19937 rng;

    void spawnScoutWave(int count);
    void spawnExplorerWave(int count);
    void spawnBoss();

public:
    LevelManager(EntityManager &em, ResourceManager &rm, sf::Vector2f bounds);

    void update(float dt);

    void notifyEnemyDeath(sf::Vector2f pos);

    void trySpawnPowerUp(sf::Vector2f pos);

    int getCurrentLevel() const { return currentLevel; }
};