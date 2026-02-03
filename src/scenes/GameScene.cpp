#include "GameScene.h"
#include "../core/Game.h"

#include "../entities/ships/SpaceShip.h"
#include "../entities/ships/enemies/Scout.h"

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
            auto& p = manager.add<LaserProjectile>(direction, speed, dmg, tex, Faction::Player);
            p.setPosition(pos);
        }
        else if (type == ProjectileType::MISSILE) 
        {
            const sf::Texture& missileTex = resources.getTexture("../assets/img/Federation_Shot_2.png");
            auto& m = manager.add<MissileProjectile>(direction, speed, 1000.f, dmg, missileTex, Faction::Player);
            m.setPosition(pos);
        } });

    player->setPosition({size.x / 2.f, (size.y / 2.f) + (size.y / 2.7f)});

    spawnEnemyWave(10);
}

void GameScene::spawnEnemyWave(int count)
{

    std::uniform_real_distribution<float> distX(50.f, window.getSize().x - 50.f);
    std::uniform_real_distribution<float> distY(-2000.f, -100.f);
    std::uniform_real_distribution<float> distFireRate(0.5f, 1.5f);

    const sf::Texture &scoutTex = resources.getTexture("../assets/img/Klingon_Ship_1.png");

    auto enemyFireAction = [this](ProjectileType type, const sf::Vector2f &pos, int dmg, float speed)
    {
        sf::Vector2f direction(0.f, 1.f);
        const sf::Texture &tex = resources.getTexture("../assets/img/Klingon_Shot_1.png");
        auto &p = manager.add<LaserProjectile>(direction, speed, dmg, tex, Faction::Alien);
        p.setPosition(pos);
    };

    for (int i = 0; i < count; ++i)
    {
        float x = distX(rng);
        float y = distY(rng);

        auto &enemy = manager.add<Scout>(scoutTex, sf::Vector2f(x, y));

        enemy.setFireRate(distFireRate(rng));
        enemy.setFireCallback(enemyFireAction);
    }
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
    collisionManager.checkCollisions(manager);

    manager.refresh();
}

void GameScene::render()
{
    window.clear(sf::Color::Black);
    manager.draw(window);
    window.display();
}