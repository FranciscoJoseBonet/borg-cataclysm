#include "Game.h"
#include <iostream>
#include "../scenes/GameScene.h"
#include "../scenes/GameOverScene.h"
#include "../scenes/ScoreScene.h"
#include "../scenes/MenuScene.h"

Game::Game()
{
    window.create(sf::VideoMode({res.width, res.height}), "Borg Cataclysm");
    window.setFramerateLimit(144);

    currentScene = std::make_unique<MenuScene>(window);
}

void Game::run()
{
    while (window.isOpen())
    {
        currentScene->handleEvents();
        currentScene->update();
        currentScene->render();

        SceneType next = currentScene->getNextScene();

        if (next != SceneType::None)
        {
            if (next == SceneType::Game)
            {
                currentScene = std::make_unique<GameScene>(window);
            }
            else if (next == SceneType::Score)
            {
                currentScene = std::make_unique<ScoreScene>(window);
            }
            else if (next == SceneType::GameOver)
            {
                int finalScore = currentScene->getScore();
                currentScene = std::make_unique<GameOverScene>(window, finalScore);
            }
            else if (next == SceneType::Menu)
            {
                currentScene = std::make_unique<MenuScene>(window);
            }
            else if (next == SceneType::Options)
            {
                std::cout << "Opciones no implementado aun.\n";
            }
        }
    }
}