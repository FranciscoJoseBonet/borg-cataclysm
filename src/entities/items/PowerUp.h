#pragma once
#include "../Entity.h"
#include <SFML/Graphics.hpp>
#include <optional>

enum class PowerUpType
{
    SHIELD,
    RAPID_FIRE,
    DOUBLE_SHOT,
    INVINCIBILITY,
    HEAL,
    EXTRA_LIFE
};

class PowerUp : public Entity
{
private:
    PowerUpType powerUpType;
    float speed;
    std::optional<sf::Sprite> sprite;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    PowerUp(sf::Vector2f position, PowerUpType type, const sf::Texture &texture);

    void update(float deltaTime) override;
    sf::FloatRect getBounds() const override;

    PowerUpType getPowerUpType() const { return powerUpType; }
};