#pragma once
#include "Scene.h"
#include "../entities/ships/SpaceShip.h"
#include "../managers/EntityManager.h"

class GameScene : public Scene
{
private:
    sf::RenderWindow &window;
    EntityManager manager;
    SpaceShip *player = nullptr;
    sf::Clock clock;

public:
    GameScene(sf::RenderWindow &window);
    void handleEvents() override;
    void update() override;
    void render() override;
};