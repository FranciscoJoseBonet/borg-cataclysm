#include "MissileProjectile.h"
#include <iostream>
#include <cmath>

MissileProjectile::MissileProjectile(
    sf::Vector2f direction,
    float initialSpeed,
    float maxSpeed,
    int damage,
    const sf::Texture &texture,
    Faction faction)
    : Projectile(direction, initialSpeed, damage, texture, faction, ProjectileType::MISSILE),
      maxSpeed(maxSpeed),
      acceleration(1000.f),
      currentSpeed(initialSpeed)
{
    // Ajuste visual especifico del misil
    sprite.setScale({0.4f, 0.4f});
}

void MissileProjectile::update(float deltaTime)
{
    // Acelera hasta llegar a velocidad maxima
    if (currentSpeed < maxSpeed)
    {
        currentSpeed += acceleration * deltaTime;

        // Actualizamos la variable speed heredada del padre para que el move() funcione
        speed = currentSpeed;
    }

    // Movimiento base del proyectil, direccion * velocidad
    move(direction * speed * deltaTime); // Move es de sfml
}