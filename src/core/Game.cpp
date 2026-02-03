#include "Game.h"
#include "../scenes/GameScene.h"

Game::Game()
{
    window.create(sf::VideoMode({res.width, res.height}), "Borg Cataclysm");
    window.setFramerateLimit(144);
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