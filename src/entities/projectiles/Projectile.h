#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "../Entity.h"

class Projectile : public Entity
{
protected:
    sf::Vector2f direction;
    float speed;
    int damage;

    sf::Texture texture;
    std::optional<sf::Sprite> sprite;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    Projectile(sf::Vector2f direction, float speed, int damage);
    virtual ~Projectile() = default;

    void update(float deltaTime) override;

    int getDamage() const { return damage; }
};
