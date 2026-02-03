#include "GameScene.h"
#include "../core/Game.h"
#include "../entities/ships/SpaceShip.h"

#include "../entities/projectiles/LaserProjectile.h"
#include "../entities/projectiles/MissileProjectile.h"

GameScene::GameScene(sf::RenderWindow &w) : window(w)
{
    auto size = window.getSize();

    player = &manager.add<SpaceShip>();

    player->setWeaponsCallback([this](ProjectileType type, const sf::Vector2f &pos, int dmg, float speed)
                               {
        sf::Vector2f direction(0.f, -1.f);

        if (type == ProjectileType::LASER) 
        {
            const sf::Texture& tex = resources.getTexture("../assets/img/Federation_Shot_1.png");
            auto& p = manager.add<LaserProjectile>(direction, speed, dmg, tex);
            p.setPosition(pos);
        }
        else if (type == ProjectileType::MISSILE) 
        {
            const sf::Texture& missileTex = resources.getTexture("../assets/img/Federation_Shot_2.png");
            auto& m = manager.add<MissileProjectile>(direction, speed, 800.f, dmg, missileTex);
            m.setPosition(pos);
        } });

    player->setPosition({size.x / 2.f, (size.y / 2.f) + (size.y / 2.7f)});
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
    manager.update(dt);
    manager.refresh();
}

void GameScene::render()
{
    window.clear(sf::Color::Black);
    manager.draw(window);
    window.display();
}