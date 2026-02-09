#include "Game.h"
#include <iostream>

// Includes de Escenas
#include "../scenes/GameScene.h"
#include "../scenes/GameOverScene.h"
#include "../scenes/ScoreScene.h"
#include "../scenes/MenuScene.h"
#include "../scenes/SelectionScene.h"

Game::Game()
{
    // Config inicial y arrancamos en Fullscreen detectando la resolución nativa
    auto desktopMode = sf::VideoMode::getDesktopMode();
    isFullscreen = true;

    window.create(desktopMode, "Borg Cataclysm", sf::Style::Default, sf::State::Fullscreen);
    window.setFramerateLimit(144);

    // Iniciamos con el Menu
    currentScene = std::make_unique<MenuScene>(window);
}

void Game::toggleFullscreen()
{
    isFullscreen = !isFullscreen;

    // SFML nos pide recrear la ventana para cambiar el estilo
    if (isFullscreen)
    {
        auto mode = sf::VideoMode::getDesktopMode();
        window.create(mode, "Borg Cataclysm", sf::Style::Default, sf::State::Fullscreen);
    }
    else
    {
        // Modo ventana a 720p
        window.create(sf::VideoMode({1280, 720}), "Borg Cataclysm", sf::Style::Default);
    }

    window.setFramerateLimit(144);
}

void Game::run()
{
    // Game Loop - Aca es donde vive el juego, el gestor de las escenas, el director
    while (window.isOpen())
    {
        // 1. INPUT: Polling de eventos
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Manejo global de teclas (F11 para pantalla completa)
            if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::F11)
                {
                    toggleFullscreen();
                    continue; // Saltamos el resto para evitar conflictos en este frame
                }
            }

            // Delegamos el evento a la escena activa
            if (currentScene)
            {
                currentScene->handleEvent(*event);
            }
        }

        // 2. UPDATE & RENDER: Delegación a la escena (hasta aca llegá el scope del manejo de escenas)
        if (currentScene)
        {
            currentScene->update();
            currentScene->render();

            // 3. LOGICA DE CAMBIO DE ESCENA (State Machine simple :) )
            SceneType next = currentScene->getNextScene();

            if (next != SceneType::None)
            {
                // Factory simple para cambiar de escena segun el enum retornado
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
                    // Pasamos el puntaje final antes de destruir la escena actual
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
}