#include "CollisionManager.h"
#include "../entities/projectiles/Projectile.h"
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

                if (proj->getFaction() != entityB->getFaction())
                {
                    if (dynamic_cast<Projectile *>(entityB.get()))
                        continue;

                    if (proj->getBounds().findIntersection(entityB->getBounds()))
                    {
                        entityB->takeDamage(proj->getDamage());
                        proj->destroy();

                        break;
                    }
                }
            }
        }
    }
}