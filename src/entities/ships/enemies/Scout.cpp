#include "Scout.h"
#include "../../../weapons/LaserLauncher.h"

Scout::Scout(const sf::Texture &texture, sf::Vector2f startPos)
    : Enemy(50.f, "Scout", 1.0f),
      sprite(texture)
{
    speed = 150.f;

    auto bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    sprite.setScale({0.6f, 0.6f});

    sprite.setRotation(sf::degrees(90.f));

    setPosition(startPos);

    weapon = new LaserLauncher("Alien Laser", 1.0f, 400.f, 10);
}

void Scout::movePattern(float deltaTime)
{
    move({0.f, speed * deltaTime});
}

void Scout::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

    if (isAlive())
    {
        states.transform *= getTransform();
        target.draw(sprite, states);
    }
}

sf::FloatRect Scout::getBounds() const
{
    return getTransform().transformRect(sprite.getGlobalBounds());
}