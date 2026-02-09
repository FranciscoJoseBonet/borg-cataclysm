#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <functional>
#include "EntityManager.h"
#include "ResourceManager.h"
#include "../entities/ships/SpaceShip.h"

class LevelManager
{
private:
    // Managers de gestion de logica interna
    EntityManager &entityManager;
    ResourceManager &resources;

    // Puntero al player para pasarselo al boss
    SpaceShip *playerRef = nullptr;

    // Estado del nivel actual
    sf::Vector2f worldBounds;
    int currentLevel = 1;
    int enemiesKilled = 0;
    int enemiesToSpawn = 0;
    bool waveInProgress = false;

    // Algunas utilities
    std::mt19937 rng;
    std::function<void(int)> onLevelChanged; // Callback para la UI

    // Spawners de la logica de level
    void spawnScoutWave(int count);
    void spawnExplorerWave(int count);
    void spawnBossLevel(int stage);

public:
    LevelManager(EntityManager &em, ResourceManager &rm, sf::Vector2f bounds);

    // Ciclo de vida
    void update(float dt);

    // Eventos
    void notifyEnemyDeath(sf::Vector2f pos);
    void trySpawnPowerUp(sf::Vector2f pos);

    // Setters / Getters
    void setPlayer(SpaceShip *player) { playerRef = player; }
    void setOnLevelChanged(std::function<void(int)> callback) { onLevelChanged = callback; }
    int getCurrentLevel() const { return currentLevel; }
};