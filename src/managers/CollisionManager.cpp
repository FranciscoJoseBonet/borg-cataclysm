#include "CollisionManager.h"
#include "../entities/projectiles/Projectile.h"
#include "../entities/ships/SpaceShip.h"
#include "../entities/items/PowerUp.h"
#include <iostream>

void CollisionManager::checkCollisions(EntityManager &manager)
{
    auto &entities = manager.getEntities();

    SpaceShip *player = nullptr;
    for (size_t i = 0; i < entities.size(); ++i)
    {
        if (auto p = dynamic_cast<SpaceShip *>(entities[i].get()))
        {
            player = p;
            break;
        }
    }

    for (size_t i = 0; i < entities.size(); ++i)
    {
        auto &entityA = entities[i];

        if (!entityA->isAlive())
            continue;

        Projectile *proj = dynamic_cast<Projectile *>(entityA.get());
        if (proj)
        {
            for (size_t j = 0; j < entities.size(); ++j)
            {
                if (i == j)
                    continue;

                auto &entityB = entities[j];

                if (!entityB->isAlive())
                    continue;
                if (dynamic_cast<PowerUp *>(entityB.get()))
                    continue;
                if (dynamic_cast<Projectile *>(entityB.get()))
                    continue;

                if (proj->getFaction() != entityB->getFaction())
                {
                    if (entityB.get() == player && player->isInvulnerableState())
                    {
                        continue;
                    }

                    if (proj->getBounds().findIntersection(entityB->getBounds()))
                    {
                        bool wasAlive = entityB->isAlive();
                        entityB->takeDamage(proj->getDamage());

                        if (wasAlive && !entityB->isAlive())
                        {
                            if (entityB->getFaction() == Faction::Alien && onEnemyDeath)
                            {
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
            for (size_t j = 0; j < entities.size(); ++j)
            {
                if (i == j)
                    continue;

                auto &entityB = entities[j];

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
                            float crashDamage = entityB->getHealth() / 2.f;
                            std::cout << "¡Colisión! Daño recibido: " << crashDamage << "\n";

                            player->takeDamage(crashDamage);

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