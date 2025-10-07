// Game.h
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../scenes/Scene.h"

class Game
{
private:
    sf::RenderWindow window;
    std::unique_ptr<Scene> currentScene;

public:
    Game();
    void run();
};
