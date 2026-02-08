#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include "EntityManager.h"
#include "../entities/projectiles/Projectile.h"

class CollisionManager
{
private:
    std::function<void(sf::Vector2f)> onEnemyDeath;
    std::function<void(sf::Vector2f, ProjectileType)> onProjectileImpact;

public:
    void checkCollisions(EntityManager &manager);

    void setOnEnemyDeath(std::function<void(sf::Vector2f)> callback)
    {
        onEnemyDeath = callback;
    }

    void setOnProjectileImpact(std::function<void(sf::Vector2f, ProjectileType)> callback)
    {
        onProjectileImpact = callback;
    }
};