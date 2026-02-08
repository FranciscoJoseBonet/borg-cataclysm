#include "CollisionManager.h"
#include "../entities/ships/SpaceShip.h"
#include "../entities/ships/enemies/Enemy.h"
#include "../entities/projectiles/Projectile.h"
#include "../entities/items/PowerUp.h"
#include <iostream>

void CollisionManager::checkCollisions(EntityManager &manager)
{
    auto &allEntities = manager.getEntities();

    std::vector<Projectile *> playerProjectiles;
    std::vector<Projectile *> enemyProjectiles;
    std::vector<Enemy *> enemies;
    std::vector<PowerUp *> powerups;
    SpaceShip *player = nullptr;

    for (auto &uPtr : allEntities)
    {
        if (!uPtr->isAlive())
            continue;

        Entity *e = uPtr.get();

        if (auto p = dynamic_cast<SpaceShip *>(e))
        {
            player = p;
            continue;
        }

        if (auto proj = dynamic_cast<Projectile *>(e))
        {
            if (proj->getFaction() == Faction::Player)
                playerProjectiles.push_back(proj);
            else
                enemyProjectiles.push_back(proj);
            continue;
        }

        if (auto enemy = dynamic_cast<Enemy *>(e))
        {
            enemies.push_back(enemy);
            continue;
        }

        if (auto pu = dynamic_cast<PowerUp *>(e))
        {
            powerups.push_back(pu);
            continue;
        }
    }

    for (auto proj : playerProjectiles)
    {
        if (!proj->isAlive())
            continue;
        sf::FloatRect projBounds = proj->getBounds();

        for (auto enemy : enemies)
        {
            if (!enemy->isAlive())
                continue;

            if (projBounds.findIntersection(enemy->getBounds()))
            {
                proj->destroy();
                enemy->takeDamage(static_cast<float>(proj->getDamage()));

                if (onProjectileImpact)
                {
                    onProjectileImpact(proj->getPosition(), proj->getType());
                }

                if (enemy->getHealth() <= 0)
                {
                    if (onEnemyDeath)
                        onEnemyDeath(enemy->getPosition());
                    enemy->destroy();
                }
                break;
            }
        }
    }

    if (!player || !player->isAlive())
        return;
    sf::FloatRect playerBounds = player->getBounds();

    for (auto pu : powerups)
    {
        if (!pu->isAlive())
            continue;

        if (playerBounds.findIntersection(pu->getBounds()))
        {
            player->applyPowerUp(pu->getPowerType());
            pu->destroy();
        }
    }

    for (auto enemy : enemies)
    {
        if (!enemy->isAlive())
            continue;

        if (playerBounds.findIntersection(enemy->getBounds()))
        {
            if (player->isInvulnerableState())
            {
                enemy->takeDamage(1000.f);
                if (onEnemyDeath)
                    onEnemyDeath(enemy->getPosition());
            }
            else
            {
                float damage = enemy->getHealth() / 2.f;
                player->takeDamage(damage);

                enemy->takeDamage(1000.f);
                if (onEnemyDeath)
                    onEnemyDeath(enemy->getPosition());
            }
        }
    }

    for (auto proj : enemyProjectiles)
    {
        if (!proj->isAlive())
            continue;

        if (playerBounds.findIntersection(proj->getBounds()))
        {
            if (!player->isInvulnerableState())
            {
                player->takeDamage(static_cast<float>(proj->getDamage()));
            }
            proj->destroy();
        }
    }
}