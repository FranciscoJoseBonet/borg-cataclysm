#pragma once
#include <string>
#include "../entities/projectiles/Projectile.h"

class Weapon
{

private:
    std::string name;
    float fireRate;
    float coolDown;

public:
    Weapon(const std::string &name, float fireRate, float cd)
        : name(name), fireRate(fireRate), coolDown(cd) {}
    virtual ~Weapon() = default;

    Projectile *Shoot(const sf::Vector2f &startPos);

    std::string getName() const { return name; }
    float getFireRate() const { return fireRate; }
    float getCd() const { return coolDown; }
};