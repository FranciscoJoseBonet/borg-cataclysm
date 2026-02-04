#include "CollisionManager.h"
#include "../entities/projectiles/Projectile.h"
#include "../entities/ships/SpaceShip.h"
#include "../entities/items/PowerUp.h"
#include <iostream>

void CollisionManager::checkCollisions(EntityManager &manager)
{
    auto &entities = manager.getEntities();

    SpaceShip *player = nullptr;
    for (auto &e : entities)
    {
        if (auto p = dynamic_cast<SpaceShip *>(e.get()))
        {
            player = p;
            break;
        }
    }

    for (auto &entityA : entities)
    {
        if (!entityA->isAlive())
            continue;

        Projectile *proj = dynamic_cast<Projectile *>(entityA.get());
        if (proj)
        {
            for (auto &entityB : entities)
            {
                if (entityA == entityB)
                    continue;
                if (!entityB->isAlive())
                    continue;
                if (dynamic_cast<PowerUp *>(entityB.get()))
                    continue;
                if (dynamic_cast<Projectile *>(entityB.get()))
                    continue;

                if (proj->getFaction() != entityB->getFaction())
                {
                    if (entityB.get() == player)
                    {
                        if (player->isInvulnerableState())
                        {
                            continue;
                        }
                    }

                    if (proj->getBounds().findIntersection(entityB->getBounds()))
                    {
                        bool wasAlive = entityB->isAlive();

                        entityB->takeDamage(proj->getDamage());

                        if (wasAlive && !entityB->isAlive())
                        {
                            if (entityB->getFaction() == Faction::Alien)
                            {
                                if (onEnemyDeath)
                                    onEnemyDeath(entityB->getPosition());
                            }
                        }

                        proj->destroy();
                        break;
                    }
                }
            }
        }

        if (player && entityA.get() == player)
        {
            for (auto &entityB : entities)
            {
                if (entityA == entityB)
                    continue;
                if (!entityB->isAlive())
                    continue;

                if (auto powerUp = dynamic_cast<PowerUp *>(entityB.get()))
                {
                    if (player->getBounds().findIntersection(powerUp->getBounds()))
                    {
                        player->applyPowerUp(powerUp->getPowerType());
                        powerUp->destroy();
                    }
                }

                else if (entityB->getFaction() == Faction::Alien && !dynamic_cast<Projectile *>(entityB.get()))
                {
                    if (player->getBounds().findIntersection(entityB->getBounds()))
                    {
                        if (player->isInvulnerableState())
                        {
                            entityB->takeDamage(1000.f);
                            if (onEnemyDeath)
                                onEnemyDeath(entityB->getPosition());
                        }
                        else
                        {
                            float enemyHealth = entityB->getHealth();
                            float crashDamage = enemyHealth / 2.f;

                            player->takeDamage(crashDamage);
                            std::cout << "¡Colisión! Daño recibido: " << crashDamage << "\n";

                            entityB->takeDamage(1000.f);
                            if (onEnemyDeath)
                                onEnemyDeath(entityB->getPosition());
                        }
                    }
                }
            }
        }
    }
}