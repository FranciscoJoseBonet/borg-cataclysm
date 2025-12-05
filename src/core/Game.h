#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../scenes/Scene.h"

struct Resolution
{
    unsigned int width;
    unsigned int height;
};

class Game
{
private:
    sf::RenderWindow window;
    Resolution res{1280, 600};
    std::unique_ptr<Scene> currentScene;

public:
    Game();
    void run();
};