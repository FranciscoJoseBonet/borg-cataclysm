#include "Game.h"
#include <iostream>
#include "../scenes/GameScene.h"
#include "../scenes/GameOverScene.h"
#include "../scenes/ScoreScene.h"
#include "../scenes/MenuScene.h"
#include "../scenes/SelectionScene.h"

Game::Game()
{
    auto desktopMode = sf::VideoMode::getDesktopMode();
    res.width = desktopMode.size.x;
    res.height = desktopMode.size.y;

    window.create(desktopMode, "Borg Cataclysm", sf::Style::Default, sf::State::Fullscreen);
    window.setFramerateLimit(144);

    currentScene = std::make_unique<MenuScene>(window);
}

void Game::toggleFullscreen()
{
    isFullscreen = !isFullscreen;

    if (isFullscreen)
    {
        auto mode = sf::VideoMode::getDesktopMode();
        window.create(mode, "Borg Cataclysm", sf::Style::Default, sf::State::Fullscreen);
    }
    else
    {
        window.create(sf::VideoMode({1280, 720}), "Borg Cataclysm", sf::Style::Default);
    }

    window.setFramerateLimit(144);
}

void Game::run()
{
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::F11)
                {
                    toggleFullscreen();
                    continue;
                }
            }

            if (currentScene)
            {
                currentScene->handleEvent(*event);
            }
        }

        currentScene->update();
        currentScene->render();

        SceneType next = currentScene->getNextScene();
        if (next != SceneType::None)
        {
            if (next == SceneType::Game)
            {
                currentScene = std::make_unique<GameScene>(window);
            }
            else if (next == SceneType::Selection)
            {
                currentScene = std::make_unique<SelectionScene>(window);
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
        }
    }
}