#pragma once
#include "../../Entity.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <functional>
#include "../../projectiles/Projectile.h"

using OnFireCallback = std::function<void(ProjectileType, const sf::Vector2f &, int, float)>;

class Enemy : public Entity
{
protected:
    float fireTimer;
    float fireInterval;
    float speed;

    float projectileSpeed;
    int damage;

    std::optional<sf::Sprite> sprite;
    OnFireCallback onFire;

public:
    Enemy(const sf::Texture &texture, float health, const std::string &type, float fireRate);
    virtual ~Enemy();

    void setFireCallback(OnFireCallback callback);
    void setFireRate(float rate);

    void setProjectileSpeed(float s);

    void setSpeed(float s) { speed = s; }
    float getSpeed() const { return speed; }

    void update(float deltaTime) override;

    virtual void movePattern(float deltaTime) {}

    sf::FloatRect getBounds() const override;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};