#pragma once
#include "EntityManager.h"
#include <functional>

class CollisionManager
{
private:
    std::function<void(sf::Vector2f)> onEnemyDeath;
    std::function<void(sf::Vector2f)> onProjectileImpact;

public:
    void checkCollisions(EntityManager &manager);

    void setOnEnemyDeath(std::function<void(sf::Vector2f)> callback)
    {
        onEnemyDeath = callback;
    }

    void setOnProjectileImpact(std::function<void(sf::Vector2f)> callback)
    {
        onProjectileImpact = callback;
    }
};