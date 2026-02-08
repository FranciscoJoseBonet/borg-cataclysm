#include "LevelManager.h"

#include "../entities/ships/enemies/Scout.h"
#include "../entities/ships/enemies/Explorer.h"
#include "../entities/projectiles/LaserProjectile.h"
#include "../entities/projectiles/CurvedProjectile.h"
#include "../entities/items/PowerUp.h"

#include <iostream>

LevelManager::LevelManager(EntityManager &em, ResourceManager &rm, sf::Vector2f bounds)
    : entityManager(em), resources(rm), worldBounds(bounds), rng(std::random_device{}())
{
    enemiesToSpawn = 5;
}

void LevelManager::update(float dt)
{
    bool anyEnemyAlive = false;
    auto &allEntities = entityManager.getEntities();

    for (const auto &ent : allEntities)
    {
        if (ent->isAlive() && dynamic_cast<Enemy *>(ent.get()) != nullptr)
        {
            anyEnemyAlive = true;
            break;
        }
    }

    if (!anyEnemyAlive && enemiesToSpawn <= 0)
    {
        currentLevel++;
        std::cout << "Iniciando Nivel: " << currentLevel << std::endl;

        if (currentLevel == 1)
        {
            spawnScoutWave(10);
        }
        else if (currentLevel == 2)
        {
            spawnScoutWave(8);
            spawnExplorerWave(4);
        }
        else
        {
            int count = 10 + (currentLevel * 2);
            spawnScoutWave(count);
            if (currentLevel > 3)
                spawnExplorerWave(currentLevel);
        }
    }
    else if (!waveInProgress && enemiesToSpawn > 0)
    {
        spawnScoutWave(enemiesToSpawn);
        enemiesToSpawn = 0;
    }
}

void LevelManager::spawnScoutWave(int count)
{
    std::uniform_real_distribution<float> distX(50.f, worldBounds.x - 50.f);
    std::uniform_real_distribution<float> distY(-1200.f, -100.f);

    std::uniform_real_distribution<float> distSpeed(80.f, 180.f);
    std::uniform_real_distribution<float> distFireRate(0.5f, 2.5f);
    std::uniform_real_distribution<float> distProjSpeed(300.f, 500.f);

    const sf::Texture &scoutTex = resources.getTexture("../assets/img/ships/Klingon_Ship_1.png");

    auto enemyFireAction = [this](ProjectileType type, const sf::Vector2f &pos, int dmg, float speed)
    {
        sf::Vector2f direction(0.f, 1.f);
        const sf::Texture &tex = resources.getTexture("../assets/img/Shots/Laser/Klingon_Shot_1.png");

        auto &p = entityManager.add<LaserProjectile>(direction, speed, dmg, tex, Faction::Alien);
        p.setPosition(pos);
    };

    for (int i = 0; i < count; ++i)
    {
        float x = distX(rng);
        float y = distY(rng);

        auto &enemy = entityManager.add<Scout>(scoutTex, sf::Vector2f(x, y));

        enemy.setFireCallback(enemyFireAction);

        enemy.setSpeed(distSpeed(rng));
        enemy.setFireRate(distFireRate(rng));
        enemy.setProjectileSpeed(distProjSpeed(rng));
    }
    waveInProgress = true;
}

void LevelManager::spawnExplorerWave(int count)
{

    std::uniform_real_distribution<float> distX(150.f, worldBounds.x - 150.f);
    std::uniform_real_distribution<float> distY(-1500.f, -200.f);

    const sf::Texture &romulanTex = resources.getTexture("../assets/img/ships/Romulan_Explorer.png");

    auto romulanFireAction = [this](ProjectileType type, const sf::Vector2f &pos, int dmg, float speed)
    {
        const sf::Texture &tex = resources.getTexture("../assets/img/Shots/Missile/Romulan_Shot_2.png");

        sf::Vector2f dir(0.f, 1.f);

        entityManager.add<CurvedProjectile>(pos, dir, speed, dmg, 5.0f, tex, Faction::Alien);
        entityManager.add<CurvedProjectile>(pos, dir, speed, dmg, -5.0f, tex, Faction::Alien);
    };

    for (int i = 0; i < count; ++i)
    {
        float x = distX(rng);
        float y = distY(rng);

        auto &enemy = entityManager.add<Explorer>(romulanTex, sf::Vector2f(x, y));

        enemy.setFireCallback(romulanFireAction);
        enemy.setProjectileSpeed(250.f);
    }
    waveInProgress = true;
}

void LevelManager::notifyEnemyDeath(sf::Vector2f pos)
{
    enemiesKilled++;
    trySpawnPowerUp(pos);
}

void LevelManager::trySpawnPowerUp(sf::Vector2f position)
{
    std::uniform_real_distribution<float> chance(0.f, 1.f);
    if (chance(rng) > 0.4f)
        return;

    std::uniform_int_distribution<int> distChance(0, 105);
    int roll = distChance(rng);

    PowerUpType type;

    if (roll <= 35)
        type = PowerUpType::SHIELD;
    else if (roll <= 65)
        type = PowerUpType::RAPID_FIRE;
    else if (roll <= 85)
        type = PowerUpType::DOUBLE_SHOT;
    else if (roll <= 94)
        type = PowerUpType::INVINCIBILITY;
    else if (roll <= 100)
        type = PowerUpType::HEAL;
    else
        type = PowerUpType::EXTRA_LIFE;

    std::string texturePath;
    switch (type)
    {
    case PowerUpType::SHIELD:
        texturePath = "../assets/img/PowerUps/PU_Extra_Shield.png";
        break;
    case PowerUpType::DOUBLE_SHOT:
        texturePath = "../assets/img/PowerUps/PU_Double_Shot.png";
        break;
    case PowerUpType::RAPID_FIRE:
        texturePath = "../assets/img/PowerUps/PU_Rapid_Fire.png";
        break;
    case PowerUpType::INVINCIBILITY:
        texturePath = "../assets/img/PowerUps/PU_Invencibility.png";
        break;
    case PowerUpType::HEAL:
        texturePath = "../assets/img/PowerUps/PU_Heal.png";
        break;
    case PowerUpType::EXTRA_LIFE:
        texturePath = "../assets/img/PowerUps/PU_Extra_Life_Rainbow.png";
        break;
    }

    const sf::Texture &tex = resources.getTexture(texturePath);
    entityManager.add<PowerUp>(position, type, tex);
}