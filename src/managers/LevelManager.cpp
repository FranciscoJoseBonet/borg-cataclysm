#include "LevelManager.h"

#include "../entities/ships/enemies/Scout.h"
#include "../entities/ships/enemies/Explorer.h"
#include "../entities/ships/enemies/BorgCube.h"

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

        if (onLevelChanged)
        {
            onLevelChanged(currentLevel);
        }

        if (currentLevel % 3 == 0)
        {
            int bossStage = currentLevel / 3;
            spawnBossLevel(bossStage);
        }
        else
        {
            if (currentLevel == 1)
            {
                spawnScoutWave(6);
            }
            else if (currentLevel == 2)
            {
                spawnScoutWave(5);
                spawnExplorerWave(1);
            }
            else
            {
                int scoutCount = 6 + (currentLevel / 2);
                spawnScoutWave(scoutCount);
                int explorerCount = 1 + (currentLevel / 3);
                spawnExplorerWave(explorerCount);
            }
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
    std::uniform_real_distribution<float> distY(-1600.f, -300.f);

    std::uniform_int_distribution<int> distArchetype(0, 100);

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

        int roll = distArchetype(rng);

        float finalSpeed = 0.f;
        float finalFireRate = 0.f;
        float finalProjSpeed = 400.f;

        if (roll < 60) // 60% STANDARD
        {
            std::uniform_real_distribution<float> s(100.f, 140.f);
            std::uniform_real_distribution<float> f(2.0f, 3.0f);
            finalSpeed = s(rng);
            finalFireRate = f(rng);
        }
        else if (roll < 85) // 25% RUSHER
        {
            std::uniform_real_distribution<float> s(160.f, 200.f);
            std::uniform_real_distribution<float> f(3.0f, 4.0f);
            finalSpeed = s(rng);
            finalFireRate = f(rng);
            finalProjSpeed = 450.f;
        }
        else // 15% GUNNER
        {
            std::uniform_real_distribution<float> s(80.f, 100.f);
            std::uniform_real_distribution<float> f(1.5f, 2.0f);
            finalSpeed = s(rng);
            finalFireRate = f(rng);
            finalProjSpeed = 350.f;
        }

        enemy.setSpeed(finalSpeed);
        enemy.setFireRate(finalFireRate);
        enemy.setProjectileSpeed(finalProjSpeed);
    }
    waveInProgress = true;
}

void LevelManager::spawnExplorerWave(int count)
{
    std::uniform_real_distribution<float> distX(100.f, worldBounds.x - 100.f);
    std::uniform_real_distribution<float> distY(-1800.f, -400.f);

    std::uniform_real_distribution<float> distSpeed(70.f, 100.f);
    std::uniform_real_distribution<float> distFireRate(2.5f, 5.0f);

    const sf::Texture &romulanTex = resources.getTexture("../assets/img/ships/Romulan_Explorer.png");

    auto romulanFireAction = [this](ProjectileType type, const sf::Vector2f &pos, int dmg, float speed)
    {
        const sf::Texture &tex = resources.getTexture("../assets/img/Shots/Missile/Romulan_Shot_2.png");
        sf::Vector2f dir(0.f, 1.f);

        std::uniform_real_distribution<float> distCurveFreq(1.5f, 3.0f);
        std::uniform_real_distribution<float> distProjSpeedVar(0.6f, 0.8f);

        float freq1 = distCurveFreq(rng);
        float freq2 = distCurveFreq(rng);
        float speedVar = distProjSpeedVar(rng);

        entityManager.add<CurvedProjectile>(pos, dir, speed * speedVar, dmg, freq1, tex, Faction::Alien);
        entityManager.add<CurvedProjectile>(pos, dir, speed * speedVar, dmg, -freq2, tex, Faction::Alien);
    };

    for (int i = 0; i < count; ++i)
    {
        float x = distX(rng);
        float y = distY(rng);

        auto &enemy = entityManager.add<Explorer>(romulanTex, sf::Vector2f(x, y));

        enemy.setFireCallback(romulanFireAction);

        enemy.setSpeed(distSpeed(rng));
        enemy.setFireRate(distFireRate(rng));
        enemy.setProjectileSpeed(240.f);
    }
    waveInProgress = true;
}

void LevelManager::spawnBossLevel(int stage)
{
    enemiesToSpawn = 0;
    const sf::Texture &bossTex = resources.getTexture("../assets/img/ships/Borg_Cube.png");

    float startX = worldBounds.x / 2.f;
    float startY = -300.f;

    auto &boss = entityManager.add<BorgCube>(bossTex, sf::Vector2f(startX, startY), stage, worldBounds.x);

    auto bossFireAction = [this, stage](ProjectileType type, const sf::Vector2f &pos, int dmg, float speed)
    {
        const sf::Texture &tex = resources.getTexture("../assets/img/Shots/Missile/Borg_Shot_3.png");

        sf::Vector2f targetDir(0.f, 1.f);

        if (playerRef && playerRef->isAlive())
        {
            sf::Vector2f playerPos = playerRef->getPosition();
            sf::Vector2f diff = playerPos - pos;

            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (length != 0)
            {
                targetDir = diff / length;
                speed *= 1.5f;
            }
        }

        auto spawnShot = [&](sf::Vector2f direction)
        {
            auto &p = entityManager.add<LaserProjectile>(direction, speed, dmg, tex, Faction::Alien);
            p.setPosition(pos);
            float angleDeg = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;

            p.setRotation(sf::degrees(angleDeg - 90.f));
        };

        spawnShot(targetDir);

        if (stage >= 2)
        {
            spawnShot(sf::Vector2f(-0.5f, 0.8f));
            spawnShot(sf::Vector2f(0.5f, 0.8f));
        }
        if (stage >= 3)
        {
            spawnShot(sf::Vector2f(-0.8f, 0.5f));
            spawnShot(sf::Vector2f(0.8f, 0.5f));
        }
    };

    boss.setFireCallback(bossFireAction);
    boss.setProjectileSpeed(350.f);
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