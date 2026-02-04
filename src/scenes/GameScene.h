#pragma once
#include <random>

#include "Scene.h"

#include "../environment/StarField.h"

#include "../entities/ships/SpaceShip.h"
#include "../entities/ships/enemies/Scout.h"
#include "../entities/items/PowerUp.h"

#include "../managers/EntityManager.h"
#include "../managers/ResourceManager.h"
#include "../managers/CollisionManager.h"

class GameScene : public Scene
{
private:
    sf::RenderWindow &window;
    ResourceManager resources;
    EntityManager manager;
    CollisionManager collisionManager;
    std::mt19937 rng;

    sf::Clock deltaClock;

    StarField stars;

    SpaceShip *player = nullptr;
    bool gameOver = false;
    int score = 0;

    void spawnEnemyWave(int count);
    void spawnPowerUp(sf::Vector2f position);

public:
    GameScene(sf::RenderWindow &window);

    void handleEvents() override;
    void update() override;
    void render() override;

    SceneType getNextScene() const override
    {
        return gameOver ? SceneType::GameOver : SceneType::None;
    }

    int getScore() const override { return score; }
};