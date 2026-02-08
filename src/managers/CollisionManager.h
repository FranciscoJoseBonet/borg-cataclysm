#pragma once
#include "EntityManager.h"
#include "../entities/projectiles/Projectile.h"
#include <functional>
#include <SFML/Graphics.hpp>

class CollisionManager
{
public:
    void checkCollisions(EntityManager &entityManager);

    void setOnEnemyDeath(std::function<void(sf::Vector2f)> callback);
    void setOnProjectileImpact(std::function<void(sf::Vector2f, ProjectileType)> callback);

private:
    void resolveCollision(Entity *a, Entity *b);

    std::function<void(sf::Vector2f)> onEnemyDeath;
    std::function<void(sf::Vector2f, ProjectileType)> onProjectileImpact;
};