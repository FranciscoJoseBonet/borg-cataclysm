#include "MissileProjectile.h"
#include <iostream>
#include <cmath>

MissileProjectile::MissileProjectile(
    sf::Vector2f direction,
    float initialSpeed,
    float maxSpeed,
    int damage)
    : Projectile(direction, initialSpeed, damage),
      maxSpeed(maxSpeed),
      acceleration(1000.f)
{
    sf::Image img;
    if (!img.loadFromFile("../assets/img/Federation_Shot_2.png"))
    {
        std::cerr << "Error: No se pudo cargar el asset del misil\n";
        img = sf::Image({4, 12}, sf::Color::Red);
    }

    if (!texture.loadFromImage(img))
    {
        std::cerr << "Error: No se pudo crear la textura del misil\n";
        return;
    }

    sprite.emplace(texture);

    auto bounds = sprite->getLocalBounds();
    sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    sprite->setScale({0.7f, 0.7f});

    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    sprite->setRotation(sf::degrees(angle));
}

void MissileProjectile::update(float deltaTime)
{
    if (speed < maxSpeed)
        speed += acceleration * deltaTime;

    move(direction * speed * deltaTime);
}

void MissileProjectile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (!sprite)
        return;

    states.transform *= getTransform();
    target.draw(*sprite, states);
}
