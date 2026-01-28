#include "LaserProjectile.h"
#include <iostream>
#include <cmath>

LaserProjectile::LaserProjectile(
    sf::Vector2f direction,
    float speed,
    int damage)
    : Projectile(direction, speed, damage)
{

    sf::Image img;
    if (!img.loadFromFile("../assets/img/Federation_Shot_1.png"))
    {
        std::cerr << "No se pudo cargar la textura del laser\n";
        img = sf::Image({2, 10}, sf::Color::Blue);
    }

    texture.loadFromImage(img);
    sprite.emplace(texture);

    auto bounds = sprite->getLocalBounds();
    sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    sprite->setScale({0.5f, 0.5f});

    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    sprite->setRotation(sf::degrees(angle));
}
