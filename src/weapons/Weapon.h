#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include "../entities/projectiles/Projectile.h"

class Weapon
{
protected:
    std::string name;
    float cooldownTime;
    float projectileSpeed;
    int damage;
    float currentCooldown;

public:
    Weapon(const std::string &name, float fireRate, float speed, int dmg);
    virtual ~Weapon() = default;

    virtual Projectile *Shoot(const sf::Vector2f &startPos);

    void update(float deltaTime);

    std::string getName() const { return name; }
    float getFireRate() const { return (cooldownTime > 0) ? (1.f / cooldownTime) : 0.f; }
};