#include "GameScene.h"
#include "../core/Game.h"

#include "../entities/ships/SpaceShip.h"
#include "../entities/ships/enemies/Scout.h"

#include "../entities/projectiles/LaserProjectile.h"
#include "../entities/projectiles/MissileProjectile.h"
#include "../entities/items/PowerUp.h"

GameScene::GameScene(sf::RenderWindow &w)
    : window(w), rng(std::random_device{}())
{
    auto size = window.getSize();

    player = &manager.add<SpaceShip>();

    player->setWeaponsCallback([this](ProjectileType type, const sf::Vector2f &pos, int dmg, float speed)
                               {
        sf::Vector2f direction(0.f, -1.f);

        if (type == ProjectileType::LASER) 
        {
            const sf::Texture& tex = resources.getTexture("../assets/img/Federation_Shot_1.png");
            
            if (player->isDoubleShotActive())
            {
                auto& p1 = manager.add<LaserProjectile>(direction, speed, dmg, tex, Faction::Player);
                p1.setPosition({pos.x - 25.f, pos.y}); 
                auto& p2 = manager.add<LaserProjectile>(direction, speed, dmg, tex, Faction::Player);
                p2.setPosition({pos.x + 25.f, pos.y});
            }
            else
            {
                auto& p = manager.add<LaserProjectile>(direction, speed, dmg, tex, Faction::Player);
                p.setPosition(pos);
            }

        }
        else if (type == ProjectileType::MISSILE) 
        {
            const sf::Texture& missileTex = resources.getTexture("../assets/img/Federation_Shot_2.png");
            
            auto& m = manager.add<MissileProjectile>(direction, speed, 1000.f, dmg, missileTex, Faction::Player);
            m.setPosition(pos);
        } });

    player->setPosition({size.x / 2.f, (size.y / 2.f) + (size.y / 2.7f)});

    collisionManager.setOnEnemyDeath([this](sf::Vector2f deathPos)
                                     {
        std::uniform_real_distribution<float> chanceDist(0.f, 1.f);
        
        if (chanceDist(rng) <= 0.4f)                //Chance de dropear un poweruppppppppppp
        {
            spawnPowerUp(deathPos);
        } });

    spawnEnemyWave(25);
}

void GameScene::spawnEnemyWave(int count)
{
    std::uniform_real_distribution<float> distX(50.f, window.getSize().x - 50.f);
    std::uniform_real_distribution<float> distY(-2000.f, -100.f);
    std::uniform_real_distribution<float> distFireRate(0.5f, 1.5f);
    std::uniform_real_distribution<float> distShipSpeed(80.f, 150.f);
    std::uniform_real_distribution<float> distProjSpeed(300.f, 600.f);

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

        enemy.setSpeed(distShipSpeed(rng));
        enemy.setFireRate(distFireRate(rng));
        enemy.setProjectileSpeed(distProjSpeed(rng));

        enemy.setFireCallback(enemyFireAction);
        enemy.setHealth(30.f);
    }
}

void GameScene::spawnPowerUp(sf::Vector2f position)
{
    std::uniform_int_distribution<int> distChance(0, 100);
    int roll = distChance(rng);

    PowerUpType type;

    // 0  a 40  (40% chance) -> SHIELD
    // 41 a 70  (30% chance) -> RAPID_FIRE
    // 71 a 90  (20% chance) -> DOUBLE_SHOT
    // 91 a 100 (10% chance) -> INVINCIBILITY

    if (roll <= 40)
    {
        type = PowerUpType::INVINCIBILITY;
    }
    else if (roll <= 70)
    {
        type = PowerUpType::RAPID_FIRE;
    }
    else if (roll <= 90)
    {
        type = PowerUpType::DOUBLE_SHOT;
    }
    else
    {
        type = PowerUpType::INVINCIBILITY;
    }

    std::string texturePath;
    switch (type)
    {
    case PowerUpType::SHIELD:
        texturePath = "../assets/img/PU_Extra_Shield.png";
        break;
    case PowerUpType::DOUBLE_SHOT:
        texturePath = "../assets/img/PU_Double_Shot.png";
        break;
    case PowerUpType::RAPID_FIRE:
        texturePath = "../assets/img/PU_Rapid_Fire.png";
        break;
    case PowerUpType::INVINCIBILITY:
        texturePath = "../assets/img/PU_Invencibility.png";
        break;
    }

    const sf::Texture &tex = resources.getTexture(texturePath);

    manager.add<PowerUp>(position, type, tex);
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