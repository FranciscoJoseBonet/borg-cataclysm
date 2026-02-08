#include "LevelManager.h"
#include "../entities/ships/enemies/Scout.h"
#include "../entities/projectiles/LaserProjectile.h"
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
        // PASAR DE NIVEL
        currentLevel++;
        std::cout << "Iniciando Nivel: " << currentLevel << std::endl;

        if (currentLevel == 1)
        {
            spawnScoutWave(10);
        }
        else if (currentLevel == 2)
        {
            spawnScoutWave(10);
            // spawnExplorerWave(5); // Descomentar en Fase 2
        }
        else
        {
            // Nivel Infinito
            int count = 10 + (currentLevel * 2);
            spawnScoutWave(count);
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
    std::uniform_real_distribution<float> distY(-1000.f, -100.f);
    std::uniform_real_distribution<float> distSpeed(80.f, 150.f);

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

        enemy.setSpeed(distSpeed(rng));
        enemy.setFireCallback(enemyFireAction);
        enemy.setHealth(30.f);
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