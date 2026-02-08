#include "Explorer.h"
#include <cmath>

Explorer::Explorer(const sf::Texture &texture, sf::Vector2f startPos)
    : Enemy(texture, 60.f, "Explorer", 1.5f)
{
    initialX = startPos.x;
    setPosition(startPos);
    setSpeed(90.f);

    if (sprite)
    {
        sprite->setScale({0.4f, 0.4f});
    }
}

void Explorer::update(float dt)
{
    timeAlive += dt;
    Enemy::update(dt);
}

void Explorer::movePattern(float dt)
{

    float newX = initialX + std::sin(timeAlive * frequency) * amplitude;
    float newY = getPosition().y + (getSpeed() * dt);

    setPosition({newX, newY});
}