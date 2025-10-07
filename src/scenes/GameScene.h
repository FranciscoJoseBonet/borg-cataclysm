// GameScene.h
#pragma once
#include "Scene.h"
#include "../entities/Player.h"

class GameScene : public Scene
{
private:
    sf::RenderWindow &window;
    Player player;

public:
    GameScene(sf::RenderWindow &window);
    void handleEvents() override;
    void update() override;
    void render() override;
};
