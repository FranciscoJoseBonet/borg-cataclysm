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
    Resolution res;
    std::unique_ptr<Scene> currentScene;

    bool isFullscreen = true;

    void toggleFullscreen();

public:
    Game();
    void run();

    sf::RenderWindow &getWindow() { return window; }
};