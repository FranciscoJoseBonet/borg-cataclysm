#include "GameScene.h"

GameScene::GameScene(sf::RenderWindow &w) : window(w) {}

void GameScene::handleEvents()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void GameScene::update()
{
    static sf::Clock deltaClock;
    float dt = deltaClock.restart().asSeconds();
    spaceShip.update(dt);
}

void GameScene::render()
{
    window.clear(sf::Color::Black);
    window.draw(spaceShip);
    window.display();
}
