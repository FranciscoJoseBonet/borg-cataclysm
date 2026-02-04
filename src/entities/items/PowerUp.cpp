#include "PowerUp.h"

PowerUp::PowerUp(sf::Vector2f position, PowerUpType type, const sf::Texture &texture)
    : Entity(1, "PowerUp", Faction::Neutral),
      powerType(type),
      speed(100.f)
{
    sprite.emplace(texture);

    setPosition(position);

    sprite->setScale({0.30f, 0.30f});

    auto bounds = sprite->getLocalBounds();
    sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
}

void PowerUp::update(float deltaTime)
{
    move({0.f, speed * deltaTime});

    if (getPosition().y > 2000.f)
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
        return getTransform().transformRect(sprite->getGlobalBounds());
    return sf::FloatRect();
}