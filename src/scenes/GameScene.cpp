#include "GameScene.h"
#include "../core/Game.h"
#include "../entities/ships/SpaceShip.h"

GameScene::GameScene(sf::RenderWindow &w) : window(w)
{

    auto size = window.getSize();

    spaceShip.setPosition({size.x / 2.f, (size.y / 2.f) + (size.y / 2.7f)});
}

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
