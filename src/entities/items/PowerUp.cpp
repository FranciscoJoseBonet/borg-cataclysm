#include "PowerUp.h"

PowerUp::PowerUp(sf::Vector2f position, PowerUpType type, const sf::Texture &texture)
    : Entity(1.f, "PowerUp", Faction::Neutral),
      powerUpType(type),
      speed(100.f)
{
    setPosition(position);

    sprite.emplace(texture);
    if (sprite)
    {
        auto bounds = sprite->getLocalBounds();
        sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        sprite->setScale({0.3f, 0.3f});
    }
}

void PowerUp::update(float deltaTime)
{
    move({0.f, speed * deltaTime});

    if (getPosition().y > 1000.f)
    {
        destroy();
    }
}

void PowerUp::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (sprite)
    {
        states.transform *= getTransform();
        target.draw(*sprite, states);
    }
}

sf::FloatRect PowerUp::getBounds() const
{
    if (sprite)
    {
        return getTransform().transformRect(sprite->getGlobalBounds());
    }
    return sf::FloatRect();
}