#pragma once
#include "Scene.h"
#include "../entities/ships/SpaceShip.h"
#include "../managers/EntityManager.h"
#include "../managers/ResourceManager.h"

class GameScene : public Scene
{
private:
    sf::RenderWindow &window;
    EntityManager manager;
    ResourceManager resources;
    SpaceShip *player = nullptr;
    sf::Clock clock;

public:
    GameScene(sf::RenderWindow &window);
    void handleEvents() override;
    void update() override;
    void render() override;
};