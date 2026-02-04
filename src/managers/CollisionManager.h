#pragma once
#include "EntityManager.h"
#include <functional>

class CollisionManager
{
public:
    using OnEnemyDeathCallback = std::function<void(sf::Vector2f)>;

    void checkCollisions(EntityManager &entities);
    void setOnEnemyDeath(OnEnemyDeathCallback callback) { onEnemyDeath = callback; }

private:
    OnEnemyDeathCallback onEnemyDeath;
};