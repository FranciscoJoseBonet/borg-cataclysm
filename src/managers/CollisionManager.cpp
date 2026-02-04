#include "CollisionManager.h"
#include "../entities/projectiles/Projectile.h"
#include "../entities/ships/SpaceShip.h"
#include "../entities/items/PowerUp.h"
#include <iostream>

void CollisionManager::checkCollisions(EntityManager &manager)
{
    auto &entities = manager.getEntities();

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

                if (proj->getFaction() != entityB->getFaction())
                {
                    if (dynamic_cast<Projectile *>(entityB.get()))
                        continue;

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

        SpaceShip *player = dynamic_cast<SpaceShip *>(entityA.get());
        if (player)
        {
            for (auto &entityB : entities)
            {
                if (!entityB->isAlive())
                    continue;

                PowerUp *powerUp = dynamic_cast<PowerUp *>(entityB.get());

                if (powerUp)
                {
                    if (player->getBounds().findIntersection(powerUp->getBounds()))
                    {
                        std::cout << "bandera poweruppppp |||| AAAAAAAAAAAAAAAAAAAAA\n";
                        player->applyPowerUp(powerUp->getPowerType());

                        powerUp->destroy();
                    }
                }
            }
        }
    }
}
