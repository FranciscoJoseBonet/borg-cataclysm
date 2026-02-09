#pragma once
#include "EntityManager.h"
#include "../entities/projectiles/Projectile.h"
#include <functional>
#include <SFML/Graphics.hpp>

class CollisionManager
{
public:
    // Metodo principal que recorre las entidades buscando choques
    void checkCollisions(EntityManager &entityManager);

    // Setters para definir que pasa externamente (como sumar puntos, particles)
    void setOnEnemyDeath(std::function<void(sf::Vector2f)> callback);
    void setOnProjectileImpact(std::function<void(sf::Vector2f, ProjectileType)> callback);

private:
    // Logica interna de que pasa cuando dos cosas chocan
    void resolveCollision(Entity *a, Entity *b);

    // Callbacks
    std::function<void(sf::Vector2f)> onEnemyDeath;
    std::function<void(sf::Vector2f, ProjectileType)> onProjectileImpact;
};