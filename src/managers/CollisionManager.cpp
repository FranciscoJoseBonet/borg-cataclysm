#include "CollisionManager.h"
#include "../entities/ships/SpaceShip.h"
#include "../entities/ships/enemies/Enemy.h"
#include "../entities/projectiles/Projectile.h"
#include "../entities/items/PowerUp.h"
#include <iostream>

void CollisionManager::checkCollisions(EntityManager &entityManager)
{
    auto &entities = entityManager.getEntities();

    for (auto itA = entities.begin(); itA != entities.end(); ++itA)
    {
        for (auto itB = std::next(itA); itB != entities.end(); ++itB)
        {
            Entity *a = itA->get();
            Entity *b = itB->get();

            if (!a->isAlive() || !b->isAlive())
                continue;

            SpaceShip *player = nullptr;
            Entity *other = nullptr;

            if (auto *s = dynamic_cast<SpaceShip *>(a))
            {
                player = s;
                other = b;
            }
            else if (auto *s = dynamic_cast<SpaceShip *>(b))
            {
                player = s;
                other = a;
            }

            if (player && player->isInvulnerableState())
            {
                if (other->getFaction() == Faction::Alien)
                    continue;
            }

            if (dynamic_cast<Projectile *>(a) && dynamic_cast<Projectile *>(b))
            {
                continue;
            }

            if (a->getFaction() != b->getFaction())
            {
                if (a->getBounds().findIntersection(b->getBounds()))
                    resolveCollision(a, b);
            }
            else
            {
                if ((dynamic_cast<SpaceShip *>(a) && dynamic_cast<PowerUp *>(b)) ||
                    (dynamic_cast<SpaceShip *>(b) && dynamic_cast<PowerUp *>(a)))
                {
                    if (a->getBounds().findIntersection(b->getBounds()))
                        resolveCollision(a, b);
                }
            }
        }
    }
}

void CollisionManager::resolveCollision(Entity *a, Entity *b)
{
    Projectile *proj = dynamic_cast<Projectile *>(a);
    Entity *target = b;

    if (!proj)
    {
        proj = dynamic_cast<Projectile *>(b);
        target = a;
    }

    if (proj)
    {
        if (dynamic_cast<PowerUp *>(target))
        {
            return;
        }

        target->takeDamage((float)proj->getDamage());
        proj->destroy();

        if (onProjectileImpact)
        {
            onProjectileImpact(proj->getPosition(), proj->getType());
        }

        if (!target->isAlive() && target->getFaction() == Faction::Alien)
        {
            if (onEnemyDeath)
                onEnemyDeath(target->getPosition());
        }
        return;
    }

    if (a->getFaction() == Faction::Player && b->getFaction() == Faction::Alien)
    {
        a->takeDamage(50.f);
        b->takeDamage(100.f);
        if (!b->isAlive() && onEnemyDeath)
            onEnemyDeath(b->getPosition());
        return;
    }
    else if (b->getFaction() == Faction::Player && a->getFaction() == Faction::Alien)
    {
        b->takeDamage(50.f);
        a->takeDamage(100.f);
        if (!a->isAlive() && onEnemyDeath)
            onEnemyDeath(a->getPosition());
        return;
    }

    SpaceShip *ship = dynamic_cast<SpaceShip *>(a);
    PowerUp *powerup = dynamic_cast<PowerUp *>(b);

    if (!ship)
    {
        ship = dynamic_cast<SpaceShip *>(b);
        powerup = dynamic_cast<PowerUp *>(a);
    }

    if (ship && powerup)
    {
        ship->applyPowerUp(powerup->getPowerUpType());
        powerup->destroy();
    }
}

void CollisionManager::setOnEnemyDeath(std::function<void(sf::Vector2f)> callback)
{
    onEnemyDeath = callback;
}

void CollisionManager::setOnProjectileImpact(std::function<void(sf::Vector2f, ProjectileType)> callback)
{
    onProjectileImpact = callback;
}