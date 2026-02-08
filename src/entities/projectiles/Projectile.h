#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "../Entity.h"

enum class ProjectileType
{
    LASER,
    MISSILE
};

class Projectile : public Entity
{
protected:
    sf::Vector2f direction;
    float speed;
    int damage;

    ProjectileType type;

    std::optional<sf::Sprite> sprite;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    Projectile(sf::Vector2f direction, float speed, int damage, Faction faction, ProjectileType type);

    sf::FloatRect getBounds() const override;

    int getDamage() const { return damage; }

    ProjectileType getType() const { return type; }

    virtual ~Projectile() = default;

    void update(float deltaTime) override;

    void destroy() override;
};