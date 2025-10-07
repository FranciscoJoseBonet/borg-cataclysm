// Game.cpp
#include "Game.h"
#include "../scenes/GameScene.h"

Game::Game()
    : window(sf::VideoMode({1280, 600}), "Borg Cataclysm")
{
    window.setFramerateLimit(60);
    currentScene = std::make_unique<GameScene>(window);
}

void Game::run()
{
    while (window.isOpen())
    {
        currentScene->handleEvents();
        currentScene->update();
        currentScene->render();
    }
}
