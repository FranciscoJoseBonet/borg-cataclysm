#pragma once
#include "../Entity.h"

enum class PowerUpType
{
    SHIELD,
    DOUBLE_SHOT,
    RAPID_FIRE,
    INVINCIBILITY,
    EXTRA_LIFE,
    HEAL
};

class PowerUp : public Entity
{
public:
    PowerUp(sf::Vector2f position, PowerUpType type, const sf::Texture &texture);

    void update(float deltaTime) override;
    sf::FloatRect getBounds() const override;

    PowerUpType getPowerType() const { return powerType; }

    void destroy() override { Entity::destroy(); }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    PowerUpType powerType;
    std::optional<sf::Sprite> sprite;
    float speed;
};