#include "LaserProjectile.h"
#include <iostream>
#include <cmath>

LaserProjectile::LaserProjectile(
    sf::Vector2f direction,
    float speed,
    int damage,
    const sf::Texture &texture,
    Faction faction)
    // Inicializamos al Padre pasandole todos los datos
    : Projectile(direction, speed, damage, texture, faction, ProjectileType::LASER)
{

    // Hacemos mas chiquito el laser (los assets estan casi todos iguales por eso modificamos esto aca)
    sprite.setScale({0.25f, 0.25f});
}