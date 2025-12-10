#include "Projectile.h"
#include <iostream>
#include <cmath>

Projectile::Projectile(sf::Vector2f direction, float speed, int damage)
    : Entity(1.f, "Federation"), sprite(texture)
{
    this->direction = direction;
    this->speed = speed;
    this->damage = damage;

    sf::Image img;

    if (!img.loadFromFile("../assets/img/Federation_Shot_1.png"))
    {
        std::cerr << "No se pudo cargar la textura del jugador\n";
        img = sf::Image({2, 10}, sf::Color::Blue);
    }

    if (!texture.loadFromImage(img))
        std::cerr << "No se pudo cargar textura desde imagen\n";

    sprite = sf::Sprite(texture);

    sprite.setPosition(getPosition());
    sprite.setScale({0.5f, 0.5f});
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;
    sprite.setRotation(sf::degrees(angle + 0.f));
}

void Projectile::update(float deltaTime)
{
    sf::Vector2f movement = direction * speed * deltaTime;
    move(movement);
}

void Projectile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(sprite, states);
}
