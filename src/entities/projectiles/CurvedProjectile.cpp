#include "CurvedProjectile.h"
#include <cmath>

CurvedProjectile::CurvedProjectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, int damage, float curveFrequency, const sf::Texture &texture, Faction faction)
    : Projectile(direction, speed, damage, faction, ProjectileType::LASER)
{
    this->curveFrequency = curveFrequency;
    this->curveWidth = 3.0f;
    this->timeAlive = 0.f;
    this->maxLifetime = 9.0f;

    setPosition(startPos);

    sprite.emplace(texture);
    if (sprite)
    {
        auto bounds = sprite->getLocalBounds();
        sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    }
}

void CurvedProjectile::update(float dt)
{
    timeAlive += dt;

    if (timeAlive > maxLifetime)
    {
        destroy();
        return;
    }

    float sideMovement = std::cos(timeAlive * curveFrequency) * curveWidth;

    sf::Vector2f moveVector = direction;
    moveVector.x += sideMovement;

    move(moveVector * speed * dt);

    if (sprite)
    {
        float rotation = std::atan2(moveVector.y, moveVector.x) * 180.f / 3.14159265f;
        sprite->setRotation(sf::degrees(rotation + 90.f));
    }

    sf::Vector2f pos = getPosition();
    if (pos.y > 1500.f || pos.y < -500.f || pos.x < -500.f || pos.x > 2000.f)
    {
        destroy();
    }
}