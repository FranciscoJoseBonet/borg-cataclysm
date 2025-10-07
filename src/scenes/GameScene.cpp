// GameScene.cpp
#include "GameScene.h"
#include <iostream>
#include <memory>

GameScene::GameScene(sf::RenderWindow &w) : window(w) {}

void GameScene::handleEvents()
{
    while (auto event = window.pollEvent())
    {
        // Cerrar ventana
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void GameScene::update()
{
    player.update();
}

void GameScene::render()
{
    window.clear(sf::Color::Black);
    player.draw(window);
    window.display();
}
