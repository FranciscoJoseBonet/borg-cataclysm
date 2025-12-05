#pragma once
#include "Scene.h"
#include "../entities/SpaceShip.h"

class GameScene : public Scene
{
private:
    sf::RenderWindow &window;
    SpaceShip spaceShip;
    sf::Clock clock;

public:
    GameScene(sf::RenderWindow &window);
    void handleEvents() override;
    void update() override;
    void render() override;
};
