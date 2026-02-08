#include "Scout.h"

Scout::Scout(const sf::Texture &texture, sf::Vector2f startPos)
    : Enemy(texture, 30.f, "Scout", 1.0f)
{
    setSpeed(150.f);

    setPosition(startPos);

    if (sprite)
    {
        sprite->setScale({0.6f, 0.6f});
        sprite->setRotation(sf::degrees(90.f));
    }
}

void Scout::movePattern(float deltaTime)
{
    move({0.f, getSpeed() * deltaTime});
}