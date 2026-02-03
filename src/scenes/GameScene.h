#pragma once
#include <random>

#include "Scene.h"
#include "../entities/ships/SpaceShip.h"
#include "../entities/ships/enemies/Scout.h"

#include "../managers/EntityManager.h"
#include "../managers/ResourceManager.h"
#include "../managers/CollisionManager.h"

class GameScene : public Scene
{
private:
    sf::RenderWindow &window;
    EntityManager manager;
    ResourceManager resources;
    CollisionManager collisionManager;
    SpaceShip *player = nullptr;
    sf::Clock clock;
    std::mt19937 rng;
    void spawnEnemyWave(int count);

public:
    GameScene(sf::RenderWindow &window);
    void handleEvents() override;
    void update() override;
    void render() override;
};